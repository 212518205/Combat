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
}


void UInventorySystemComponent::AddItem_Implementation(UInventoryItemInstance* InItemInstance)
{
	if (!InItemInstance || !InItemInstance->GetItemDef())
	{
		DebugPrintInventory();
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
}


void UInventorySystemComponent::DebugPrintInventory()
{if (!GEngine) return;

	if (InventoryItems.Items.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("背包为空"));
		UE_LOG(LogTemp, Warning, TEXT("Inventory is empty."));
		return;
	}

	const FString Header = FString::Printf(TEXT("===== 背包物品 (%d 件) ====="), InventoryItems.Items.Num());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Header);
	UE_LOG(LogTemp, Log, TEXT("%s"), *Header);

	for (int32 i = 0; i < InventoryItems.Items.Num(); ++i)
	{
		const UInventoryItemInstance* Item = InventoryItems.Items[i].ItemInstance;
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
