// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemTrait.h"
#include "ItemTrait_Stack.generated.h"

class UInventorySystem;
/**
 * 
 */
UCLASS()
class KITSUNE_API UItemTrait_Stack : public UInventoryItemTrait
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStackCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MinStackCount;

};
