// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySystemComponent.h"

#include "FrontendDebugHelper.h"
#include "Engine/AssetManager.h"
#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemInstance.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "Game/GameInstanceSubsystem/KitsuneSaveSubsystem.h"
#include "Game/SaveGame/KitsuneSaveGame.h"
#include "Inventory/Trait/ItemTrait_Display.h"
#include "Inventory/Trait/ItemTrait_Stack.h"
#include "Net/UnrealNetwork.h"


void FInventoryItemArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	if (!Owner)return;
	
	for (const int32 Index : AddedIndices)
	{
		if (UInventoryItemInstance* ItemInstance = Items[Index].ItemInstance)
		{
			Owner->ItemChanged.Broadcast(ItemInstance, EInstanceModifyType::AddItem);
		}
	}
}

void FInventoryItemArray::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	if (!Owner)return;
	
	for (const int32 Index : ChangedIndices)
	{
		if (UInventoryItemInstance* ItemInstance = Items[Index].ItemInstance)
		{
			Owner->ItemChanged.Broadcast(ItemInstance, EInstanceModifyType::AddStackCount);
		}
	}
}

void FInventoryItemArray::PreReplicatedRemove(const TArrayView<int32> RemoveIndices, int32 FinalSize)
{
	if (!Owner)return;
	
	for (const int32 Index : RemoveIndices)
	{
		if (UInventoryItemInstance* ItemInstance = Items[Index].ItemInstance)
		{
			Owner->ItemChanged.Broadcast(ItemInstance, EInstanceModifyType::RemoveItem);
		}
	}
}

void UInventorySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InventoryItems.Owner = this;
	if (GetOwner()->HasAuthority())
	{
		if (UKitsuneSaveSubsystem* SaveSubsystem = UKitsuneSaveSubsystem::GetSaveSubsystem(GetOwner()))
		{
			SaveSubsystem->RegisterForSaving(this);
			Debug::Print(TEXT("存档系统已注册背包库存"));
		}
	}
}

void UInventorySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (GetOwner()->HasAuthority())
	{
		if (UKitsuneSaveSubsystem* SaveSubsystem = UKitsuneSaveSubsystem::GetSaveSubsystem(GetOwner()))
		{
			SaveSubsystem->UnRegisterForSaving(this);
			Debug::Print(TEXT("存档系统已取消注册背包库存"));
		}
	}
}

void UInventorySystemComponent::AddItem_Implementation(UInventoryItemInstance* InItemInstance)
{
	if (!InItemInstance || !InItemInstance->GetItemDef())
	{
		return;
	}

	const UInventoryItemDefinition* InItemDef = InItemInstance->ItemDef;
	const int32 InItemFeature = InItemInstance->ItemFeatures;
	const int32 InStackCount = InItemInstance->StackCount;
	const UItemTrait_Stack* InStackTrait = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Stack>(InItemDef);
	const bool bIsStackable = InStackTrait && InItemInstance->HasFeature(EItemFeature::Stackable);
	const int32 ToAdd = FMath::Max(InStackCount, 1);

	if (bIsStackable)
	{
		for (FInventoryItemEntry& Entry : InventoryItems.Items)
		{
			if (UInventoryItemInstance* Item = Entry.ItemInstance;
				Item && Item->GetItemDef() == InItemDef && Item->ItemFeatures == InItemFeature)
			{
				Item->StackCount += ToAdd;
				InventoryItems.MarkItemDirty(Entry);
				if (InItemInstance->GetOwningActor())
				{
					InItemInstance->GetOwningActor()->Destroy();
				}
				return;
			}
		}
	}

	UInventoryItemInstance* NewItem = InItemInstance->CreateInstanceCopy(this);

	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
	{
		AddReplicatedSubObject(NewItem);
	}

	if (InItemInstance->GetOwningActor())
	{
		InItemInstance->GetOwningActor()->Destroy();
	}

	FInventoryItemEntry& NewEntry = InventoryItems.Items.AddDefaulted_GetRef();
	NewEntry.ItemInstance = NewItem;
	InventoryItems.MarkItemDirty(NewEntry);

	if (GetNetMode() == NM_Standalone ||
	    (GetNetMode() == NM_ListenServer && GetOwner()))
	{
		OnRep_InventoryCapacity();
	}
}


void UInventorySystemComponent::SaveTo(UKitsuneSaveGame* SaveGame)
{
	if (!SaveGame)return;
	
	Debug::Print(TEXT("保存背包"));
	
	auto& [SaveItems, SaveInventoryCapacity] = SaveGame->Inventory;
	SaveItems.Reset();
	for (FInventoryItemEntry& Entry : InventoryItems.Items)
	{
		const UInventoryItemInstance* Instance = Entry.ItemInstance; 
		if (!Instance || !Instance->GetItemDef())
		{
			Debug::Print(TEXT("SaveTo 序列化失败"));
			continue;
		}
		SaveItems.Add(Instance->ToSerialized());
	}
	SaveInventoryCapacity = InventoryCapacity;
}

void UInventorySystemComponent::LoadFrom(const UKitsuneSaveGame* SaveGame)
{
	if (!SaveGame)return;
	
	Debug::Print(TEXT("加载背包"));
	
	auto& [SaveItems, SaveInventoryCapacity] = SaveGame->Inventory;
	InventoryCapacity = SaveInventoryCapacity;
	const UAssetManager& AM = UAssetManager::Get();
	for (const auto& Serialized : SaveItems)
	{
		UInventoryItemInstance* ItemInstance = UInventoryItemInstance::CreateFromSerialized(this, Serialized);
		if (!ItemInstance)
		{
			Debug::Print(TEXT("UInventorySystemComponent::LoadFrom加载失败"));
			continue;
		}
		
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
		{
			AddReplicatedSubObject(ItemInstance);
		}
		FInventoryItemEntry& Entry = InventoryItems.Items.AddDefaulted_GetRef();
		Entry.ItemInstance = ItemInstance;
		InventoryItems.MarkItemDirty(Entry);
	}
}

void UInventorySystemComponent::UnlockCategorySlots_Implementation(const FName CategoryID)
{
	const FInventoryInfo* Info = GetCategoryInfo(CategoryID);
	if (!Info)return;
	if (!TrySpendGold(Info->UnLockCost))return;
	
	const int32 CurrentCapacity = GetCapacityByCategoryID(CategoryID);
	const int32 NewCapacity = CurrentCapacity + Info->OnceUnlockCount;
	
	FCategoryCapacityEntry* Found = InventoryCapacity.FindByPredicate(
		[CategoryID](const FCategoryCapacityEntry& Entry)
		{
			return Entry.CategoryID == CategoryID;
		});
	if (Found)
	{
		Found->CategoryCapacity = NewCapacity;
	}
	else
	{
		auto& [EntryCategoryID, EntryCategoryCapacity] = InventoryCapacity.AddDefaulted_GetRef();
		EntryCategoryID = CategoryID;
		EntryCategoryCapacity = NewCapacity;
	}
	
	if (GetNetMode() == NM_Standalone ||
		(GetNetMode() == NM_ListenServer && GetOwner()))
	{
		OnRep_InventoryCapacity();
	}
	
}

int32 UInventorySystemComponent::GetCapacityByCategoryID(const FName CategoryID)
{
	const FInventoryInfo* Info = GetCategoryInfo(CategoryID);
	int32 ReturnCategoryCapacity = Info ? Info->InitialCapacity : 10;
	
	for (auto& [EntryCategoryID, EntryCategoryCapacity] : InventoryCapacity)
	{
		if (EntryCategoryID == CategoryID)
		{
			ReturnCategoryCapacity = EntryCategoryCapacity;
		}
	}
	
	return ReturnCategoryCapacity;
}

TArray<TPair<FName, FInventoryCategoryGroup>> UInventorySystemComponent::GetAllCategoryItem()
{
	TMap<FName, FInventoryCategoryGroup> ReturnGroup;
	TMap<FName, FInventoryInfo> CategoryInfo = UFrontendBlueprintFunctionLibrary::GetCategoryNameByModuleTag(KitsuneGameplayTags::UI_CategoryDisplay_Inventory_Item).CategoryInfo;

	for (const FInventoryItemEntry& Entry : InventoryItems.Items)
	{
		const UInventoryItemInstance* Item = Entry.ItemInstance;
		if (!Item || !Item->GetItemDef()) continue;

		if (const UItemTrait_Display* TraitDisplay = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Display>(Item->GetItemDef()))
		{
			FName CategoryID = TraitDisplay->CategoryID;
			UInventorySlotData* Data = NewObject<UInventorySlotData>(this);
			Data->ItemInstance = const_cast<UInventoryItemInstance*>(Item);
			if (CategoryInfo.Contains(CategoryID))
			{
				Data->bIsLocked = false;
				Data->UnlockCost = CategoryInfo.Find(CategoryID)->UnLockCost;
			}
			Data->CategoryID = CategoryID;
			ReturnGroup.FindOrAdd(CategoryID).CategorySlots.Add(Data);
		}
	}

	for (auto &[CategoryID, Info] : CategoryInfo)
	{
		auto& [CategoryDisplayName, CategorySlots] = ReturnGroup.FindOrAdd(CategoryID);
		CategoryDisplayName = Info.CategoryName;
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
	for (FInventoryItemEntry& Entry : InventoryItems.Items)
	{
		UInventoryItemInstance* ItemInstance = Entry.ItemInstance;
		if (const UItemTrait_Display* Trait_Display = GET_TRAIT(ItemInstance, Display))
		{
			if (CategoryID == Trait_Display->CategoryID)
			{
				UInventorySlotData* Data = NewObject<UInventorySlotData>(this);
				Data->ItemInstance = ItemInstance;
				Data->bIsLocked = false;
				Data->UnlockCost = 0;
				Data->CategoryID = CategoryID;
				Slots.Add(Data);
			}
		}
	}
	
	return Slots;
}

void UInventorySystemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UInventorySystemComponent, InventoryItems);
	DOREPLIFETIME(UInventorySystemComponent, InventoryCapacity);
}

void UInventorySystemComponent::OnRep_InventoryCapacity()
{
	CapacityChanged.Broadcast();
}

const FInventoryInfo* UInventorySystemComponent::GetCategoryInfo(const FName CategoryID)
{
	/*** TODO: 需重构... [2026年8月17日 23:42:35 来自`@BC@`] ***/
	static const TMap<FName, FInventoryInfo> CategoryInfo =
		 UFrontendBlueprintFunctionLibrary::GetCategoryNameByModuleTag(
			 KitsuneGameplayTags::UI_CategoryDisplay_Inventory_Item).CategoryInfo;

	return CategoryInfo.Find(CategoryID);
}

bool UInventorySystemComponent::TrySpendGold(int32 Cost)
{
	/*** TODO: 先默认返回true以供测试解锁背包格子使用，后续应改为从属性集获取属性来解锁... [2026年8月17日 21:52:42 来自`@BC@`] ***/
	return true;
}

