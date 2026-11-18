// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Interaction/InteractionComponent.h"

#include "FunctionLibrary/KitsuneFunctionLibrary.h"
#include "GameFramework/Pawn.h"
#include "Inventory/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/ViewModel/ViewModelWorldSubsystem.h"

void UInteractionComponent::AddInteractableItem_Implementation(UInventoryItemInstance* ItemInstance)
{
	if (ItemInstance && ItemInstance->HasFeature(EInventoryItemFeature::Interactable))
	{
		ItemOverlapChange(ItemInstance, EItemInstanceAction::EAddInstance);
	}
}

void UInteractionComponent::RemoveInteractableItem_Implementation(UInventoryItemInstance* ItemInstance)
{
	if (ItemInstance && ItemInstance->HasFeature(EInventoryItemFeature::Interactable))
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

	EnsureInteractionViewModel();
}

void UInteractionComponent::EnsureInteractionViewModel()
{
	if (const APawn* Pawn = Cast<APawn>(GetOwner()); !Pawn || !Pawn->IsLocallyControlled()) return;

	if (UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this))
	{
		VMReg->GetOrCreate<UInteractionViewModel>(GetOwner())->BindInteractionComponent(this);
	}
}

void UInteractionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void UInteractionComponent::ItemOverlapChange_Implementation(UInventoryItemInstance* ItemInstance, const EItemInstanceAction InstanceAction)
{
	// [VMREFACTOR] 数据源主动推送到 VM，组件不再自己缓存 VM
	UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this);
	UInteractionViewModel* VM = VMReg ? VMReg->Find<UInteractionViewModel>(GetOwner()) : nullptr;

	if (!VM)
	{
		// 非本机 Pawn 不会建这个 VM，属预期分支
		return;
	}

	if (InstanceAction == EItemInstanceAction::EAddInstance)
	{
		VM->AddInteractableItemInstance(ItemInstance);
	}
	else
	{
		VM->RemoveInteractableItemInstance(ItemInstance);
	}
}
