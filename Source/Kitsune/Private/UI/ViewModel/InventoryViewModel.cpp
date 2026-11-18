// [VMREFACTOR] 新增文件：背包镜像（逻辑整体从 PlayerViewModel 搬过来）
#include "UI/ViewModel/InventoryViewModel.h"

#include "FunctionLibrary/KitsuneInventoryFunctionLibrary.h"
#include "FunctionLibrary/KitsuneWidgetFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Trait/InventoryItemTrait_Display.h"
#include "Inventory/Trait/InventoryItemTrait_Stack.h"
#include "UI/DataObjects/Inventory/InventorySlotData.h"

namespace
{
	const TMap<FName, FInventoryCategoryConfig>& GetInventoryCategoryInfoMap()
	{
		static const TMap<FName, FInventoryCategoryConfig> Map =
			UKitsuneWidgetFunctionLibrary::GetCategoryNameByModuleTag(
				KitsuneGameplayTags::UI_CategoryDisplay_Inventory_Item).CategoryInfo;
		return Map;
	}
}

void UInventoryViewModel::Deinitialize()
{
	// [VMREFACTOR] IsValid：待销毁对象的 Get() 仍非空，解绑会踩到已释放的委托
	if (UInventoryComponent* ISC = CachedISC.Get(); IsValid(ISC))
	{
		ISC->CapacityChanged.RemoveAll(this);
		ISC->ItemChanged.RemoveAll(this);
	}

	CachedISC.Reset();

	Super::Deinitialize();
}

void UInventoryViewModel::BindInventoryComponent(UInventoryComponent* ISC)
{
	if (!ISC)
	{
		UE_LOG(LogTemp, Warning, TEXT("[VMREFACTOR] UInventoryViewModel: 背包组件未就绪，等待下一次 Bind"));
		return;
	}

	Deinitialize();

	ISC->CapacityChanged.AddUniqueDynamic(this, &ThisClass::HandleCategoryCapacityChanged);
	ISC->ItemChanged.AddUniqueDynamic(this, &ThisClass::HandleInventoryItemChanged);
	CachedISC = ISC;

	RebuildInventoryMirror(ISC);

	// [VMREFACTOR] 先填数据再 MarkReady：Ready 广播到达时 Widget 直接能读到有效值
	MarkReady();
}

void UInventoryViewModel::RebuildInventoryMirror(const UInventoryComponent* ISC)
{
	if (!ISC) return;

	ItemsByCategory.Reset();
	for (UInventoryItemInstance* Instance : ISC->GetAllItemInstances())
	{
		if (!Instance || !Instance->GetItemDef()) continue;

		if (const UInventoryItemTrait_Display* TraitDisplay =
			UInventoryFunctionLibrary::FindItemDefinitionTrait<UInventoryItemTrait_Display>(Instance->GetItemDef()))
		{
			ItemsByCategory.FindOrAdd(TraitDisplay->CategoryID).Items.Add(Instance);
		}
	}

	CapacityByCategory.Reset();
	for (const auto& [CategoryID, Info] : GetInventoryCategoryInfoMap())
	{
		CapacityByCategory.Add(CategoryID, ISC->GetCapacityByCategoryID(CategoryID));
	}
}

void UInventoryViewModel::HandleCategoryCapacityChanged(const FName CategoryID, const int32 CategoryCapacity)
{
	CapacityByCategory.FindOrAdd(CategoryID) = CategoryCapacity;
	CategoryCapacityChanged.Broadcast(CategoryID, CategoryCapacity);
}

void UInventoryViewModel::HandleInventoryItemChanged(UInventoryItemInstance* ItemInstance,
                                                     const EInventoryItemChangeType ChangeType)
{
	// [VMREFACTOR] 原实现做增量维护；这里统一全量重建，避免"分类变更 / 堆叠变更"漏刷
	if (const UInventoryComponent* ISC = CachedISC.Get())
	{
		RebuildInventoryMirror(ISC);
	}

	OnInventoryItemChanged.Broadcast(ItemInstance, ChangeType);
}

TArray<TPair<FName, FText>> UInventoryViewModel::GetAllInventoryCategories() const
{
	const TMap<FName, FInventoryCategoryConfig>& InfoMap = GetInventoryCategoryInfoMap();

	TArray<TPair<FName, FText>> Result;
	Result.Reserve(InfoMap.Num());
	for (const auto& [CategoryID, Info] : InfoMap)
	{
		Result.Emplace(CategoryID, Info.DisplayName);
	}

	Result.Sort([&InfoMap](const TPair<FName, FText>& A, const TPair<FName, FText>& B)
	{
		const int32 OrderA = InfoMap.Contains(A.Key) ? InfoMap[A.Key].SortOrder : INT_MAX;
		const int32 OrderB = InfoMap.Contains(B.Key) ? InfoMap[B.Key].SortOrder : INT_MAX;
		return OrderA < OrderB;
	});

	return Result;
}

TArray<UInventorySlotData*> UInventoryViewModel::GetDisplaySlotsByCategory(const FName CategoryID) const
{
	const TMap<FName, FInventoryCategoryConfig>& InfoMap = GetInventoryCategoryInfoMap();
	const FInventoryCategoryConfig* Info = InfoMap.Find(CategoryID);
	if (!Info)
	{
		return {};
	}

	int32 Capacity = Info->InitialUnlockedSlots;
	if (const int32* Found = CapacityByCategory.Find(CategoryID))
	{
		Capacity = *Found;
	}

	const int32 LockCount = Info->LockSlotCount;

	TArray<UInventorySlotData*> Slots;
	int32 UsedSlots = 0;

	if (const FInventoryCategoryItems* Mirror = ItemsByCategory.Find(CategoryID))
	{
		for (const TObjectPtr<UInventoryItemInstance>& ItemPtr : Mirror->Items)
		{
			UInventoryItemInstance* Item = ItemPtr.Get();
			if (!Item || !Item->GetItemDef()) continue;

			const UInventoryItemTrait_Stack* TraitStack =
				UInventoryFunctionLibrary::FindItemDefinitionTrait<UInventoryItemTrait_Stack>(Item->GetItemDef());
			const bool bStackable = TraitStack && TraitStack->MaxStackCount > 1;
			const int32 MaxStack = bStackable ? FMath::Max(1, TraitStack->MaxStackCount) : 1;
			int32 Remaining = FMath::Max(1, Item->GetStackCount());

			while (Remaining > 0)
			{
				UInventorySlotData* Data = NewObject<UInventorySlotData>(const_cast<UInventoryViewModel*>(this));
				Data->ItemInstance = Item;
				Data->StackCount = bStackable ? FMath::Min(MaxStack, Remaining) : 0;
				Data->bIsLocked = false;
				Data->UnlockCost = 0;
				Data->CategoryID = CategoryID;
				Slots.Add(Data);
				++UsedSlots;
				Remaining -= MaxStack;
			}
		}
	}

	/***  空格   `BC@` ***/
	const int32 EmptyCount = Capacity - UsedSlots;
	for (int32 i = 0; i < EmptyCount; ++i)
	{
		UInventorySlotData* EmptySlot = NewObject<UInventorySlotData>(const_cast<UInventoryViewModel*>(this));
		EmptySlot->bIsLocked = false;
		EmptySlot->CategoryID = CategoryID;
		Slots.Add(EmptySlot);
	}

	/***  锁格   `BC@` ***/
	for (int32 i = 0; i < LockCount; ++i)
	{
		UInventorySlotData* LockedSlot = NewObject<UInventorySlotData>(const_cast<UInventoryViewModel*>(this));
		LockedSlot->bIsLocked = true;
		LockedSlot->UnlockCost = Info->UnlockCost;
		LockedSlot->CategoryID = CategoryID;
		Slots.Add(LockedSlot);
	}

	return Slots;
}

int32 UInventoryViewModel::GetCategoryCapacity(const FName CategoryID) const
{
	return CapacityByCategory.FindRef(CategoryID);
}
