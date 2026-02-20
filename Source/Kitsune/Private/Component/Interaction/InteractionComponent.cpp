// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Interaction/InteractionComponent.h"

#include "UIManagerSubsystem.h"
#include "FunctionLibrary/KitsuneFunctionLibrary.h"

void UInteractionComponent::AddInteractableItem_Implementation(UInventoryItemInstance* ItemInstance)
{
	if (ItemInstance && ItemInstance->HasAnyFeature(EItemFeature::Interactable))
	{
		ItemOverlapChange(ItemInstance, EItemInstanceAction::EAddInstance);
	}
}

void UInteractionComponent::RemoveInteractableItem_Implementation(UInventoryItemInstance* ItemInstance)
{
	if (ItemInstance && ItemInstance->HasAnyFeature(EItemFeature::Interactable))
	{
		ItemOverlapChange(ItemInstance, EItemInstanceAction::ERemoveInstance);
	}
}

UInteractionComponent::UInteractionComponent()
{
	SetIsReplicated(true);
}

void UInteractionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void UInteractionComponent::ItemOverlapChange_Implementation(UInventoryItemInstance* ItemInstance, EItemInstanceAction InstanceAction)
{

	const ENetMode NetMode = GetNetMode();
	if (UPlayerViewModel* LocalViewModel = GetLocalPlayerViewModel(); LocalViewModel 
		&& (GetOwnerRole() == ROLE_AutonomousProxy || NetMode == NM_ListenServer || NetMode == NM_Standalone))
	{
		if (InstanceAction == EItemInstanceAction::EAddInstance)
		{
			GetLocalPlayerViewModel()->AddInteractableItemInstance(ItemInstance);
		}
		else
		{
			GetLocalPlayerViewModel()->RemoveInteractableItemInstance(ItemInstance);
		}
	}
	else
	{
		Debug::Print(TEXT("不是自主代理"));
	}
}


UPlayerViewModel* UInteractionComponent::GetLocalPlayerViewModel()
{
	if (!CachedLocalViewModel.Get())
	{
		CachedLocalViewModel = UUIManagerSubsystem::GetUIManager(GetWorld())->TryGetViewModelByActor<UPlayerViewModel>(GetOwningPawn());
	}
	return CachedLocalViewModel.Get();
}
