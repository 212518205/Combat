// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemTrait.h"
#include "Inventory/Trait/ItemTrait_Interact.h"
#include "Inventory/Trait/ItemTrait_Stack.h"
#include "InventoryItemDefinition.generated.h"

#define GET_TRAIT(ItemInstance, TraitSuffix) \
		UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_##TraitSuffix>((ItemInstance)->GetItemDef())

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

UCLASS()
class KITSUNE_API UInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template<typename ResultType = UInventoryItemTrait>
	static ResultType* FindItemDefinitionTrait(const UInventoryItemDefinition* ItemDef);
};

template <typename ResultType>
ResultType* UInventoryFunctionLibrary::FindItemDefinitionTrait(const UInventoryItemDefinition* ItemDef)
{
	static_assert(TIsDerivedFrom<ResultType, UInventoryItemTrait>::Value, "ResultType 必须是 UInventoryItemTrait 子类");

	if (ItemDef == nullptr)
	{
		return nullptr;
	}

	UInventoryItemTrait* FoundTrait = ItemDef->FindTraitByClass(ResultType::StaticClass());
	return Cast<ResultType>(FoundTrait);
}
