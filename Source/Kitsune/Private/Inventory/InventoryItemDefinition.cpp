// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemTrait.h"
#include "Net/UnrealNetwork.h"

UInventoryItemTrait* UInventoryItemDefinition::FindTraitByClass(const TSubclassOf<UInventoryItemTrait> TraitClass) const
{
	if (TraitClass)
	{
		for (UInventoryItemTrait* ItemTrait : ItemTraits)
		{
			if (ItemTrait && ItemTrait->IsA(TraitClass))
			{
				return ItemTrait;
			}
		}
	}
	return nullptr;
}

void UInventoryItemDefinition::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemTraits);
}

