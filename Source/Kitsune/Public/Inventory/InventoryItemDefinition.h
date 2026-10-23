// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemTrait.h"
#include "Inventory/Trait/InventoryItemTrait_Display.h"
#include "Inventory/Trait/InventoryItemTrait_Interact.h"
#include "Inventory/Trait/InventoryItemTrait_Stack.h"
#include "Inventory/Trait/InventoryItemTrait_Use.h"
#include "Inventory/Trait/InventoryItemTrait_Trade.h"
#include "InventoryItemDefinition.generated.h"

class UInventoryItemTrait;
/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, meta = (PrimaryAssetType = "ItemDefinition"))
class KITSUNE_API UInventoryItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Trait")
	UInventoryItemTrait* FindTraitByClass(TSubclassOf<UInventoryItemTrait> TraitClass) const ;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	TArray<TObjectPtr<UInventoryItemTrait>> ItemTraits;
};
