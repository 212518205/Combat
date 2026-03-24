// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemTrait.h"
#include "ItemTrait_Interact.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "交互"))
class KITSUNE_API UItemTrait_Interact : public UInventoryItemTrait
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText InteractPrompt;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FKey PromptKey = FKey(TEXT("F"));

	UFUNCTION(BlueprintCallable)
	FText GetPromptMessage() const;
};
