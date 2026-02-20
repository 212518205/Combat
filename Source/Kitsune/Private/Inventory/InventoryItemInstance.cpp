// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"

#include "FunctionLibrary/KitsuneFunctionLibrary.h"
#include "Net/UnrealNetwork.h"


UInventoryItemInstance::UInventoryItemInstance(): ItemFeatures(static_cast<uint8>(EItemFeature::None))
{
	InstanceID = FGuid::NewGuid();
}

void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemDef);
	DOREPLIFETIME(ThisClass, InstanceID);
	DOREPLIFETIME(ThisClass, ItemFeatures);
}
