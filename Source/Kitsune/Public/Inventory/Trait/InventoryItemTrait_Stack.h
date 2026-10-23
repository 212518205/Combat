// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemTrait.h"
#include "InventoryItemTrait_Stack.generated.h"


/**
 * 
 */
UCLASS()
class KITSUNE_API UInventoryItemTrait_Stack : public UInventoryItemTrait
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStackCount = 1;

};
