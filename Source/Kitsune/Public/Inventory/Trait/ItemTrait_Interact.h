// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemTrait.h"
#include "ItemTrait_Interact.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UItemTrait_Interact : public UInventoryItemTrait
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Prompt")
	FText InteractPrompt;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Prompt")
	FKey PromptKey = FKey(TEXT("F"));

	UFUNCTION(BlueprintCallable, Category = "Prompt")
	FText GetPromptMessage() const;
};
