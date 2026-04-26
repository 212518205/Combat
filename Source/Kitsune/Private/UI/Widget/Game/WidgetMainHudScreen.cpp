// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Game/WidgetMainHudScreen.h"

#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"
#include "UI/Widget/Components/KitsuneCommonListView.h"



void UWidgetMainHudScreen::OnInteractableItemChange_Implementation(UInventoryItemInstance* ItemInstance,
	EItemInstanceAction InstanceAction)
{
	if (!ItemInstance)return;

	check(CommonListView_Prompt);
	switch (InstanceAction)
	{
	case EItemInstanceAction::EAddInstance:
		if (CachedLocalViewModel->GetOverlappedItemInstances().Contains(ItemInstance))
		{
			CommonListView_Prompt->AddItem(ItemInstance);
		}
		break;

	case EItemInstanceAction::ERemoveInstance:
		if (!CachedLocalViewModel->GetOverlappedItemInstances().Contains(ItemInstance))
		{
			CommonListView_Prompt->RemoveItem(ItemInstance);
		}
		break;

	default:
		break;
	}

	CommonListView_Prompt->RequestRefresh();
}

UInventoryItemInstance* UWidgetMainHudScreen::GetSelectedItemInstance() const
{
	UInventoryItemInstance* ItemInstance = CommonListView_Prompt->GetSelectedItem<UInventoryItemInstance>();
	if (CommonListView_Prompt->GetListItems().Num() == 0)return nullptr;
	if (!ItemInstance)
	{
		ItemInstance = Cast<UInventoryItemInstance>(CommonListView_Prompt->GetItemAt(0));
	}
	
	return ItemInstance;
}

void UWidgetMainHudScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
	
}

void UWidgetMainHudScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
}

FReply UWidgetMainHudScreen::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!CommonListView_Prompt)return FReply::Unhandled();
	if (const float WheelDelta = InMouseEvent.GetWheelDelta(); WheelDelta != 0.f)
	{
		const int32 Offset = WheelDelta > 0.f ? -1 : 1;
		ChangeSelectionByOffset(Offset);
		return FReply::Handled();
	}
	
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

FReply UWidgetMainHudScreen::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UWidgetMainHudScreen::InitializeMainHudScreen()
{
	CachedUIManager = UUIManagerSubsystem::GetUIManager(GetOwningPlayer());
	CachedLocalViewModel = CachedUIManager->TryGetViewModelByActor<UPlayerViewModel>(GetOwningPlayerPawn());

	CachedLocalViewModel->OnInteractableItemChange.AddDynamic(this, &ThisClass::OnInteractableItemChange);

	CachedLocalViewModel->OnHealthPercentChanged.Broadcast();
	CachedLocalViewModel->OnStaminaPercentChanged.Broadcast();
}

void UWidgetMainHudScreen::ChangeSelectionByOffset(const int32 Offset) const
{
	if (!CommonListView_Prompt)return;

	const int EntryNum = CommonListView_Prompt->GetNumItems();
	if (EntryNum == 0)return;

	const UObject* CurItem = CommonListView_Prompt->GetSelectedItem();
	int CurIndex = CommonListView_Prompt->GetIndexForItem(CurItem);
	CurIndex = FMath::Max(CurIndex, 0);

	int NewIndex = (CurIndex + Offset) % EntryNum;
	if (NewIndex < 0)NewIndex += EntryNum;
	CommonListView_Prompt->SetSelectedIndex(NewIndex);
}
  