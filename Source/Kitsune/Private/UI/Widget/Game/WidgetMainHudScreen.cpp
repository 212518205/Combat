// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Game/WidgetMainHudScreen.h"

#include "FrontendDebugHelper.h"
#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/Trait/ItemTrait_Interact.h"
#include "UI/Widget/Components/KitsuneCommonListView.h"



void UWidgetMainHudScreen::OnInteractableItemChange_Implementation(UInventoryItemInstance* ItemInstance,
	EItemInstanceAction InstanceAction)
{
	if (!ItemInstance)return;

	check(CommonListView_Prompt);

	switch (InstanceAction)
	{
	case EItemInstanceAction::EAddInstance:
		if (CachedPlayerViewModel->OverlappedItemInstances.Contains(ItemInstance))
		{
			CommonListView_Prompt->AddItem(ItemInstance);
		}
		break;

	case EItemInstanceAction::ERemoveInstance:
		if (!CachedPlayerViewModel->OverlappedItemInstances.Contains(ItemInstance))
		{
			CommonListView_Prompt->RemoveItem(ItemInstance);
		}
		break;

	default:
		break;
	}

	CommonListView_Prompt->RequestRefresh();
}

void UWidgetMainHudScreen::InitializeMainHudScreen()
{
	
	CachedPlayerViewModel = UUIManagerSubsystem::GetUIManager(GetOwningPlayer())->TryGetViewModelByActor<UPlayerViewModel>(GetOwningPlayerPawn());

	CachedPlayerViewModel->OnInteractableItemChange.AddDynamic(this, &ThisClass::OnInteractableItemChange);

	CachedPlayerViewModel->OnHealthPercentChanged.Broadcast();
	CachedPlayerViewModel->OnStaminaPercentChanged.Broadcast();
}
  