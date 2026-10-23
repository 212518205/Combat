
#include "UI/Widget/Game/WidgetBackpackScreen.h"

#include "Inventory/InventoryComponent.h"
#include "UI/DataObjects/Inventory/InventorySlotData.h"  
#include "UI/ViewModel/PlayerViewModel.h"
#include "UI/Widget/Components/KitsuneCommonTileView.h"
#include "UI/Widget/Components/KitsuneTabListWidgetBase.h"

void UWidgetBackpackScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (const auto& [CategoryID, DisplayName] : GetAllCategoryItems())
	{
		if (TabList_BagCategory->GetTabButtonBaseByID(CategoryID))continue;
		TabList_BagCategory->RequestRegisterTab(CategoryID, DisplayName);
	}
	if (UPlayerViewModel* PlayerVM = GetLocalPlayerViewModel())
	{
		PlayerVM->CategoryCapacityChanged.AddUniqueDynamic(this, &ThisClass::HandleCapacityChanged);
		PlayerVM->OnInventoryItemChanged.AddUniqueDynamic(this, &ThisClass::HandleInventoryItemChanged);
	}
}

void UWidgetBackpackScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	TabList_BagCategory->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnTabSelected);
}

TArray<TPair<FName, FText>> UWidgetBackpackScreen::GetAllCategoryItems()
{
	if (const UPlayerViewModel* LocalViewModel = GetLocalPlayerViewModel())
	{
		return LocalViewModel->GetAllInventoryCategories();
	}
	return {};
}

void UWidgetBackpackScreen::OnTabSelected(const FName TabID)
{
	CurrentTabID = TabID;
	RefreshCurrentTab();
}

void UWidgetBackpackScreen::RefreshCurrentTab()
{
	if (CurrentTabID == NAME_None)return;

	if (const UPlayerViewModel* PlayerVM = GetLocalPlayerViewModel())
	{
		const TArray<UInventorySlotData*>& RawSlots = PlayerVM->GetDisplaySlotsByCategory(CurrentTabID);
		TileView_Backpack->SetListItems(RawSlots);
	}
	TileView_Backpack->RequestRefresh();
}

void UWidgetBackpackScreen::HandleCapacityChanged(FName CategoryID, int32 CategoryCapacity)
{
	if (CategoryID == CurrentTabID)
	{
		RefreshCurrentTab();
	}
}

void UWidgetBackpackScreen::HandleInventoryItemChanged(UInventoryItemInstance* Instance, EInventoryItemChangeType ChangeType)
{
	RefreshCurrentTab();
}
