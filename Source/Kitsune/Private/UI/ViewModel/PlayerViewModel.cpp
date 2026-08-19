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
    if (CarriedInventorySystem)
    {
        CarriedInventorySystem->CapacityChanged.AddDynamic(this, &ThisClass::OnCategoryCapacityChanged);
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

TArray<TPair<FName, FInventoryCategoryGroup>> UPlayerViewModel::GetAllInventoryCategories() const
{
	return CarriedInventorySystem ? CarriedInventorySystem->GetAllCategoryItem() : TArray<TPair<FName, FInventoryCategoryGroup>>();
}

TArray<UInventorySlotData*> UPlayerViewModel::GetInventoryItemsByCategory(const FName CategoryID) const
{
	return CarriedInventorySystem ? CarriedInventorySystem->GetAllItemsByCategory(CategoryID) : TArray<UInventorySlotData*>();
}

int32 UPlayerViewModel::GetCategoryCapacity(const FName CategoryID) const
{
    return CarriedInventorySystem ? CarriedInventorySystem->GetCapacityByCategoryID(CategoryID) : 0;
}

void UPlayerViewModel::OnCategoryCapacityChanged(FName CategoryID, int32 CategoryCapacity)
{
    CategoryCapacityChanged.Broadcast(CategoryID, CategoryCapacity);
}
