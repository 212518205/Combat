// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Game/WidgetMainHudScreen.h"

#include "FrontendDebugHelper.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/Trait/ItemTrait_Interact.h"
#include "UI/Widget/Components/KitsuneCommonListView.h"



void UWidgetMainHudScreen::OnInteractableItemChange_Implementation(UInventoryItemInstance* ItemInstance,
	EItemInstanceAction InstanceAction)
{
	if (!ItemInstance)return;

	check(CommonListView_Prompt);
	UItemTrait_Interact* TraitInteract = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Interact>(ItemInstance->GetItemDef(), UItemTrait_Interact::StaticClass());
	if (!TraitInteract)
	{
		Debug::Print(TEXT("无交互信息"));
	}
	switch (InstanceAction)
	{
	case EItemInstanceAction::EAddInstance:
		if (CachedPlayerViewModel->OverlappedItemInstances.Contains(ItemInstance))
		{
			CommonListView_Prompt->AddItem(TraitInteract);
		}
		break;

	case EItemInstanceAction::ERemoveInstance:
		if (!CachedPlayerViewModel->OverlappedItemInstances.Contains(ItemInstance))
		{
			CommonListView_Prompt->RemoveItem(TraitInteract);
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
  