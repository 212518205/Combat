// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Interaction/InteractionComponent.h"

#include "UIManagerSubsystem.h"
#include "FunctionLibrary/KitsuneFunctionLibrary.h"
#include "Inventory/InventorySystemComponent.h"
#include "Net/UnrealNetwork.h"

void UInteractionComponent::AddInteractableItem_Implementation(UInventoryItemInstance* ItemInstance)
{
	if (ItemInstance && ItemInstance->HasFeature(EItemFeature::Interactable))
	{
		ItemOverlapChange(ItemInstance, EItemInstanceAction::EAddInstance);
	}
}

void UInteractionComponent::RemoveInteractableItem_Implementation(UInventoryItemInstance* ItemInstance)
{
	if (ItemInstance && ItemInstance->HasFeature(EItemFeature::Interactable))
	{
		ItemOverlapChange(ItemInstance, EItemInstanceAction::ERemoveInstance);
	}
}

UInteractionComponent::UInteractionComponent()
{
	SetIsReplicatedByDefault(true);
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void UInteractionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void UInteractionComponent::ItemOverlapChange_Implementation(UInventoryItemInstance* ItemInstance, const EItemInstanceAction InstanceAction)
{

	const ENetMode NetMode = GetNetMode();
	if (const UPlayerViewModel* LocalViewModel = GetLocalPlayerViewModel(); LocalViewModel 
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
