// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySystemComponent.h"

#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemInstance.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "Inventory/Trait/ItemTrait_Display.h"
#include "Inventory/Trait/ItemTrait_Stack.h"
#include "Net/UnrealNetwork.h"

void UInventorySystemComponent::AddItem_Implementation(UInventoryItemInstance* InItemInstance, const int32 InStackCount)
{
	if (!InItemInstance)
	{
		DebugPrintInventory();
		return;
	}

	/***  传入nullptr将ItemInstance置为InItemInstance并Add到数组
	 *		初始化正确的ToStackCount`BC@` ***/
	auto ItemStack = [this, InItemInstance, InStackCount](UInventoryItemInstance* ItemInstance)
		{
			const UItemTrait_Stack* TraitStack = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Stack>(InItemInstance->GetItemDef());
			if (!TraitStack)
			{
				Debug::Print(TEXT("无法从ItemInstance中获取到有效TraitStack，在UInventorySystem::AddItem中"));
				return false;
			}

			int32 ToStackCount = InStackCount ? InStackCount : InItemInstance->StackCount;
			if (!ItemInstance)
			{
				ItemInstance = InItemInstance;
				InventoryItems.Add(ItemInstance);
				/***  暂时将堆叠数设为0，先添加进Item数组才开始遍历堆叠   `BC@` ***/
				ItemInstance->StackCount = 0;
				//ItemChanged.Broadcast(ItemInstance, EInstanceModifyType::AddItem);
			}

			while (ToStackCount > 0){
				if (ToStackCount <= TraitStack->MaxStackCount - ItemInstance->StackCount)
				{
					ItemInstance->StackCount += ToStackCount;
					//ItemChanged.Broadcast(ItemInstance, EInstanceModifyType::AddStackCount);
					ToStackCount = 0;
				}else
				{
					ToStackCount -= TraitStack->MaxStackCount - ItemInstance->StackCount;
					ItemInstance->StackCount = TraitStack->MaxStackCount;
					ItemInstance = NewObject<UInventoryItemInstance>(this);
					ItemInstance->ItemDef = InItemInstance->GetItemDef();
					ItemInstance->ItemFeatures = InItemInstance->ItemFeatures;
					ItemInstance->StackCount = 0;
					InventoryItems.Add(ItemInstance);
					//ItemChanged.Broadcast(ItemInstance, EInstanceModifyType::AddItem);
				}
			}
			return true;
		};

	for (UInventoryItemInstance* ItemInstance : InventoryItems)
	{
		if (!ItemInstance)
		{
			Debug::Print(TEXT("在添加物品时物品数组中出现ItemInstance为空"));
			continue;
		}

		if (ItemInstance->GetItemDef() == InItemInstance->GetItemDef() && ItemInstance->HasFeature(EItemFeature::Stackable))
		{
			ItemStack(ItemInstance);
			return;
		}
	}

	if (InItemInstance->HasFeature(EItemFeature::Stackable))
	{
		ItemStack(nullptr);
		return;
	}

	InventoryItems.Add(InItemInstance);
	//ItemChanged.Broadcast(InItemInstance, EInstanceModifyType::AddItem);
	if (InStackCount != 1)
	{
		Debug::Print(TEXT("该物品不可堆叠，应传入StackCount为1"));
	}
	return;
}


TArray<TPair<FName, FInventoryCategoryGroup>> UInventorySystemComponent::GetAllCategoryItem()
{
	TMap<FName, FInventoryCategoryGroup> ReturnGroup;
	TMap<FName, FInventoryInfo> CategoryInfo = UFrontendBlueprintFunctionLibrary::GetCategoryNameByModuleTag(KitsuneGameplayTags::UI_CategoryDisplay_Inventory_Item).CategoryInfo;

	for (UInventoryItemInstance* ItemInstance : InventoryItems)
	{
		if (const UItemTrait_Display* TraitDisplay = GET_TRAIT(ItemInstance, Display))
		{
			FName CategoryID = TraitDisplay->CategoryID;
			UInventorySlotData* Data = NewObject<UInventorySlotData>(this);
			Data->ItemInstance = ItemInstance;
			if (CategoryInfo.Contains(CategoryID))
			{
				Data->bIsLocked = false;
				Data->UnlockCost = CategoryInfo.Find(CategoryID)->UnLockCost;
			}
			ReturnGroup.FindOrAdd(CategoryID).CategorySlots.Add(Data);
		}
	}

	for (auto &[CategoryID, Info] : CategoryInfo)
	{
		auto& [CategoryDisplayName, CategorySlots] = ReturnGroup.FindOrAdd(CategoryID);
		CategoryDisplayName = Info.CategoryName;

		for (int32 i = 0; i < Info.EmptySlotCount; i++)
		{
			UInventorySlotData* Data = NewObject<UInventorySlotData>(this);
			CategorySlots.Add(Data);
		}
		for (int32 i = 0; i < Info.LockSlotCount; i++)
		{
			UInventorySlotData* Data = NewObject<UInventorySlotData>(this);
			Data->bIsLocked = true;
			Data->UnlockCost = Info.UnLockCost;
			CategorySlots.Add(Data);
		}
	}

	TArray<TPair<FName, FInventoryCategoryGroup>> SortedResult;
	SortedResult.Reserve(ReturnGroup.Num());
	for (auto& [CategoryID, Group] : ReturnGroup)
	{
		SortedResult.Emplace(CategoryID, MoveTemp(Group));
	}

	SortedResult.Sort([&CategoryInfo](const TPair<FName, FInventoryCategoryGroup>& A, const TPair<FName, FInventoryCategoryGroup>& B)
	{
		const int32 OrderA = CategoryInfo.Contains(A.Key) ? CategoryInfo[A.Key].SortOrder : INT_MAX;
		const int32 OrderB = CategoryInfo.Contains(B.Key) ? CategoryInfo[B.Key].SortOrder : INT_MAX;
		return OrderA < OrderB;
	});

	return SortedResult;
}

TArray<UInventorySlotData*> UInventorySystemComponent::GetAllItemsByCategory(const FName CategoryID)
{
	TMap<FName, FInventoryInfo> Info = UFrontendBlueprintFunctionLibrary::GetCategoryNameByModuleTag(KitsuneGameplayTags::UI_CategoryDisplay_Inventory_Item).CategoryInfo;
	
	TArray<UInventorySlotData*> Slots;
	for (UInventoryItemInstance* ItemInstance : InventoryItems)
	{
		if (const UItemTrait_Display* Trait_Display = GET_TRAIT(ItemInstance, Display))
		{
			if (CategoryID == Trait_Display->CategoryID)
			{
				UInventorySlotData* Data = NewObject<UInventorySlotData>(this);
				Data->ItemInstance = ItemInstance;
				Data->bIsLocked = false;
				Data->UnlockCost = 0;
				Slots.Add(Data);
			}
		}
	}
	int32 EmptyCount = 25;
	int32 LockCount = 10;
	int32 UnlockCost = 500;
	if (const FInventoryInfo* CategoryInfo = Info.Find(CategoryID))
	{
		EmptyCount = CategoryInfo->EmptySlotCount;
		LockCount = CategoryInfo->LockSlotCount;
		UnlockCost = CategoryInfo->UnLockCost;
	}
	
	for ( ; EmptyCount > 0; EmptyCount--)
	{
		UInventorySlotData* Data = NewObject<UInventorySlotData>(this);
		Data->ItemInstance = nullptr;
		Data->bIsLocked = false;
		Data->UnlockCost = 0;
		Slots.Add(Data);
	}
	
	for ( ; LockCount > 0; LockCount--)
	{
		UInventorySlotData* Data = NewObject<UInventorySlotData>(this);
		Data->ItemInstance = nullptr;
		Data->bIsLocked = true;
		Data->UnlockCost = UnlockCost;
		Slots.Add(Data);
	}
	
	return Slots;
}

void UInventorySystemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UInventorySystemComponent, InventoryItems);
}

void UInventorySystemComponent::OnRep_InventoryItems()
{
}

void UInventorySystemComponent::DebugPrintInventory()
{if (!GEngine) return;

	if (InventoryItems.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("背包为空"));
		UE_LOG(LogTemp, Warning, TEXT("Inventory is empty."));
		return;
	}

	FString Header = FString::Printf(TEXT("===== 背包物品 (%d 件) ====="), InventoryItems.Num());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Header);
	UE_LOG(LogTemp, Log, TEXT("%s"), *Header);

	for (int32 i = 0; i < InventoryItems.Num(); ++i)
	{
		const UInventoryItemInstance* Item = InventoryItems[i];
		if (!Item) continue;

		const UInventoryItemDefinition* ItemDef = Item->GetItemDef();
		if (!ItemDef) continue;

		// 使用之前定义的模板函数获取 Display Trait
		const UItemTrait_Display* DisplayTrait = 
			UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Display>(ItemDef);

		FString DisplayName = DisplayTrait ? DisplayTrait->DisplayName.ToString() : TEXT("未知物品");
		FString Description = DisplayTrait ? DisplayTrait->Description.ToString() : TEXT("无描述");
		const int32 Stack = Item->StackCount;

		FString Line = FString::Printf(TEXT("[%d] %s x%d | %s"), i, *DisplayName, Stack, *Description);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, Line);
		UE_LOG(LogTemp, Log, TEXT("%s"), *Line);
	}
}
