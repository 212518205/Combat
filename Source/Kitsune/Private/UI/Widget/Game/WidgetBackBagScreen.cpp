// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Game/WidgetBackBagScreen.h"

#include "FrontendTypes/FrontendStructTypes.h"
#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventorySystemComponent.h"
#include "Inventory/Trait/ItemTrait_Stack.h"
#include "UI/ViewModel/PlayerViewModel.h"
#include "UI/Widget/Components/KitsuneCommonTileView.h"
#include "UI/Widget/Components/KitsuneTabListWidgetBase.h"

void UWidgetBackBagScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (auto& [CategoryID, Group] : GetAllCategoryItems())
	{
		if (TabList_BagCategory->GetTabButtonBaseByID(CategoryID))continue;
		TabList_BagCategory->RequestRegisterTab(CategoryID, Group.CategoryDisplayName);
	}
}

void UWidgetBackBagScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	TabList_BagCategory->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnTabSelected);
}

TArray<TPair<FName, FInventoryCategoryGroup>> UWidgetBackBagScreen::GetAllCategoryItems()
{
	if (const UPlayerViewModel* LocalViewModel = GetLocalPlayerViewModel())
	{
		return LocalViewModel->GetAllInventoryCategories();
	}
	return {};
}

void UWidgetBackBagScreen::OnTabSelected(const FName TabID)
{
	TArray<UInventorySlotData*> ItemSlots = GetLocalPlayerViewModel()->GetInventoryItemsByCategory(TabID);
	const TArray<UInventorySlotData*> Slots = BuildCategorySlots(TabID, ItemSlots);
	TileView_BackBag->SetListItems(Slots);
	TileView_BackBag->RequestRefresh();
}

TArray<UInventorySlotData*> UWidgetBackBagScreen::BuildCategorySlots(const FName& CategoryID,
	TArray<UInventorySlotData*>& Slots)
{
	TArray<UInventorySlotData*> Result;
	for (UInventorySlotData* SlotData : Slots)
	{
		UInventoryItemInstance* Item = SlotData->ItemInstance;
		if (!Item || !Item->GetItemDef())continue;
		
		if (!Item->HasFeature(EItemFeature::Stackable))
		{
			Result.Add(SlotData);
			continue;
		}

		const UItemTrait_Stack* TraitStack = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Stack>(Item->GetItemDef());
		checkf(TraitStack, TEXT("可堆叠但无法获取到TraitStack"));
		const int32 MaxStackCount = TraitStack->MaxStackCount;
		int32 CurrentStackCount = FMath::Max(1, Item->GetStackCount());

		while (CurrentStackCount > 0)
		{
			UInventorySlotData* NewSlot = NewObject<UInventorySlotData>(this);
			NewSlot->ItemInstance = Item;
			NewSlot->StackCount = FMath::Min(MaxStackCount, CurrentStackCount);
			NewSlot->bIsLocked = false;
			NewSlot->UnlockCost = SlotData->UnlockCost;
			Result.Add(NewSlot);
			CurrentStackCount -= MaxStackCount;
		}
	}
	
	const auto [CategoryInfo] = UFrontendBlueprintFunctionLibrary::GetCategoryNameByModuleTag(KitsuneGameplayTags::UI_CategoryDisplay_Inventory_Item);
	const FInventoryInfo* Info = CategoryInfo.Find(CategoryID);
	const int32 EmptyCount = Info ? Info->EmptySlotCount : 0;
	const int32 LockCount = Info ? Info->LockSlotCount : 0;
	const int32 UnlockCost = Info ? Info->UnLockCost : 0;

	for (int32 i = 0; i < EmptyCount; ++i)
	{
		UInventorySlotData* Data = NewObject<UInventorySlotData>(this);
		Data->ItemInstance = nullptr;
		Data->bIsLocked = false;
		Result.Add(Data);
	}
	for (int32 i = 0; i < LockCount; ++i)
	{
		UInventorySlotData* Data = NewObject<UInventorySlotData>(this);
		Data->ItemInstance = nullptr;
		Data->bIsLocked = true;
		Data->UnlockCost = UnlockCost;
		Result.Add(Data);
	}

	return Result;
}

