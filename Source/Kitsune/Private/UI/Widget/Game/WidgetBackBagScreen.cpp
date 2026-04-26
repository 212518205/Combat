// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Game/WidgetBackBagScreen.h"

#include "Inventory/InventorySystem.h"
#include "UI/ViewModel/PlayerViewModel.h"
#include "UI/Widget/Components/KitsuneCommonTileView.h"
#include "UI/Widget/Components/KitsuneTabListWidgetBase.h"

void UWidgetBackBagScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (auto& [CategoryID, Group] : GetAllCategoryItems())
	{
		if (TabList_BagCategory->GetTabButtonBaseByID(CategoryID))continue;
		Debug::Print(TEXT("Value = ") + CategoryID.ToString() + TEXT("Display = ") + Group.CategoryDisplayName.ToString());
		TabList_BagCategory->RequestRegisterTab(CategoryID, Group.CategoryDisplayName);
	}
}

void UWidgetBackBagScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	TabList_BagCategory->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnTabSelected);
}

TMap<FName, FInventoryCategoryGroup> UWidgetBackBagScreen::GetAllCategoryItems()
{
	if (UPlayerViewModel* LocalViewModel = GetLocalPlayerViewModel())
	{
		if (UInventorySystem* InventorySystem = LocalViewModel->GetInventorySystem())
		{
			return InventorySystem->GetAllCategoryItem();
		}
		else
		{
			Debug::Print(TEXT("GetInventorySystem 失败"));
		}
	}
	return {};
}

void UWidgetBackBagScreen::OnTabSelected(const FName TabID)
{
	const TArray<UInventoryItemInstance*> Items = GetLocalPlayerViewModel()->GetInventorySystem()->GetAllItemsByCategory(TabID);
	TileView_BackBag->SetListItems(Items);
	TileView_BackBag->RequestRefresh();
}
