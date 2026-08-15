// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"

#include "Net/UnrealNetwork.h"


UInventoryItemInstance::UInventoryItemInstance()
{
	ItemID = FGuid::NewGuid();
}

void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemDef);
	DOREPLIFETIME(ThisClass, ItemFeatures);
	DOREPLIFETIME(ThisClass, ItemID);
	DOREPLIFETIME(ThisClass, StackCount);
}

bool UInventoryItemInstance::IsSupportedForNetworking() const
{
	return true;
}

UInventoryItemInstance* UInventoryItemInstance::CreateInstanceCopy(UObject* NewOuter) const
{
	UInventoryItemInstance* Copy = NewObject<UInventoryItemInstance>(NewOuter);
	Copy->ItemDef = ItemDef;
	Copy->ItemFeatures = ItemFeatures;
	Copy->ItemID = ItemID;
	Copy->StackCount = StackCount;
	
	return Copy;
}
