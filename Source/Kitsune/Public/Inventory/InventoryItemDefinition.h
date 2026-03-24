// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemTrait.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemDefinition.generated.h"

class UInventoryItemTrait;
/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced, meta = (PrimaryAssetType = "ItemDefinition"))
class KITSUNE_API UInventoryItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Trait")
	UInventoryItemTrait* FindTraitByClass(TSubclassOf<UInventoryItemTrait> TraitClass) const ;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated , EditDefaultsOnly, BlueprintReadWrite, Instanced)
	TArray<TObjectPtr<UInventoryItemTrait>> ItemTraits;
};

UCLASS()
class KITSUNE_API UInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template<typename ResultType = UInventoryItemTrait>
	static ResultType* FindItemDefinitionTrait(const UInventoryItemDefinition* ItemDef, const TSubclassOf<UInventoryItemTrait>& TraitClass);
};

template <typename ResultType>
ResultType* UInventoryFunctionLibrary::FindItemDefinitionTrait(const UInventoryItemDefinition* ItemDef,
	const TSubclassOf<UInventoryItemTrait>& TraitClass)
{
	static_assert(TIsDerivedFrom<ResultType, UInventoryItemTrait>::Value, "ResultType 必须是 UInventoryItemTrait 子类");

	if (ItemDef == nullptr || TraitClass == nullptr)
	{
		return nullptr;
	}

	UInventoryItemTrait* FoundTrait = ItemDef->FindTraitByClass(TraitClass);
	return Cast<ResultType>(FoundTrait);
}
