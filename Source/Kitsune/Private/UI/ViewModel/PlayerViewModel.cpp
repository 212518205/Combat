// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/PlayerViewModel.h"

#include "FrontendDebugHelper.h"
#include "IDetailTreeNode.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/Trait/ItemTrait_Display.h"
#include "Inventory/Trait/ItemTrait_Interact.h"
#include "UI/DataObjects/ListDataObjectString.h"

void UPlayerViewModel::NativeInitialize()
{
	Super::NativeInitialize();
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


//void UPlayerViewModel::RemoveOrAddInteractableItemInstance(UInventoryItemInstance* ItemInstance)
//{
//	
//	
//	if (OverlappedItemInstances.Contains(ItemInstance))
//	{
//		DebugArray.Pop();
//		OverlappedItemInstances.Remove(ItemInstance);
//		OnInteractableItemChange.Broadcast(ItemInstance, EItemInstanceAction::ERemoveInstance);
//		Debug::Print(TEXT("Remove Data From DebugArray"));
//	}
//	else
//	{
//		UListDataObjectString* DebugData = NewObject<UListDataObjectString>();
//		DebugArray.Add(DebugData);
//		Debug::Print(TEXT("Add Data From DebugArray"));
//		OverlappedItemInstances.Add(ItemInstance);
//		OnInteractableItemChange.Broadcast(ItemInstance, EItemInstanceAction::EAddInstance);
//	}
//	
//}

void UPlayerViewModel::SetPlayerWeaponIcon(const TSoftObjectPtr<UTexture2D> InWeaponIcon)
{
	WeaponIcon = InWeaponIcon;
	OnWeaponChanged.Broadcast();
}
