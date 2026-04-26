// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemTrait.h"
#include "ItemTrait_Display.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "UI显示"))
class KITSUNE_API UItemTrait_Display : public UInventoryItemTrait
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName CategoryID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText CategoryDisplayName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> DisplayIcon;

};
