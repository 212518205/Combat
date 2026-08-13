// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/PlayerViewModel.h"

#include "FrontendDebugHelper.h"
#include "IDetailTreeNode.h"
#include "Characters/KitsuneCharacter.h"
#include "Component/Interaction/InteractionComponent.h"
#include "Interfaces/PawnInteractInterface.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/Trait/ItemTrait_Display.h"
#include "Inventory/Trait/ItemTrait_Interact.h"
#include "UI/DataObjects/ListDataObjectString.h"

void UPlayerViewModel::NativeInitialize()
{
	Super::NativeInitialize();
    
    if (const AKitsuneCharacter* Character = Cast<AKitsuneCharacter>(OwningPawn))
    {
        CarriedInventorySystem = Character->GetInventorySystemComponent();
    }
    if (!CarriedInventorySystem)
    {
        Debug::Print(TEXT("CarriedInventorySystem 为空"));
    }
    
}

void UPlayerViewModel::AddInteractableItemInstance(UInventoryItemInstance* ItemInstance)
{
    if (ItemInstance && !OverlappedItemInstances.Contains(ItemInstance))
    {
        OverlappedItemInstances.Add(ItemInstance);
        OnInteractableItemChange.Broadcast(ItemInstance, EItemInstanceAction::EAddInstance);
    }
}

void UPlayerViewModel::RemoveInteractableItemInstance(UInventoryItemInstance* ItemInstance)
{
    if (ItemInstance && OverlappedItemInstances.Contains(ItemInstance))
    {
        OverlappedItemInstances.Remove(ItemInstance);
        OnInteractableItemChange.Broadcast(ItemInstance, EItemInstanceAction::ERemoveInstance);
    }
}

void UPlayerViewModel::SetPlayerWeaponIcon(const TSoftObjectPtr<UTexture2D> InWeaponIcon)
{
	WeaponIcon = InWeaponIcon;
	OnWeaponChanged.Broadcast();
}
