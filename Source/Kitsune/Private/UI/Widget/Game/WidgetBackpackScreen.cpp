#include "UI/Widget/Game/WidgetBackpackScreen.h"

#include "Inventory/InventoryComponent.h"
#include "UI/DataObjects/Inventory/InventorySlotData.h"  
// [VMREFACTOR] 从 UPlayerViewModel 切到 UInventoryViewModel
#include "UI/ViewModel/InventoryViewModel.h"
#include "UI/ViewModel/ViewModelWorldSubsystem.h"
#include "UI/Widget/Components/KitsuneCommonTileView.h"
#include "UI/Widget/Components/KitsuneTabListWidgetBase.h"

UInventoryViewModel* UWidgetBackpackScreen::GetInventoryViewModel()
{
	UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this);
	return VMReg ? VMReg->Find<UInventoryViewModel>(GetOwningPlayerPawn()) : nullptr;
}

void UWidgetBackpackScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (const auto& [CategoryID, DisplayName] : GetAllCategoryItems())
	{
		if (TabList_BagCategory->GetTabButtonBaseByID(CategoryID))continue;
		TabList_BagCategory->RequestRegisterTab(CategoryID, DisplayName);
	}

	// [VMREFACTOR] AddUnique：界面反复激活不会叠加委托
	if (UInventoryViewModel* VM = GetInventoryViewModel())
	{
		VM->CategoryCapacityChanged.AddUniqueDynamic(this, &ThisClass::HandleCapacityChanged);
		VM->OnInventoryItemChanged.AddUniqueDynamic(this, &ThisClass::HandleInventoryItemChanged);
	}
}

void UWidgetBackpackScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	TabList_BagCategory->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnTabSelected);
}

TArray<TPair<FName, FText>> UWidgetBackpackScreen::GetAllCategoryItems()
{
	if (const UInventoryViewModel* VM = GetInventoryViewModel())
	{
		return VM->GetAllInventoryCategories();
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

	if (const UInventoryViewModel* VM = GetInventoryViewModel())
	{
		const TArray<UInventorySlotData*>& RawSlots = VM->GetDisplaySlotsByCategory(CurrentTabID);
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
