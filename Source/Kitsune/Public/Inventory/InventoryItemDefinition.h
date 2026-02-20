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
UCLASS(Blueprintable)
class KITSUNE_API UInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Trait")
	UInventoryItemTrait* FindTraitByClass(TSubclassOf<UInventoryItemTrait> TraitClass) const ;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated , EditDefaultsOnly, BlueprintReadWrite, Category = "Display", meta = (AllowPrivateAccess))
	TArray<TObjectPtr<UInventoryItemTrait>> ItemTraits;
};

UCLASS()
class KITSUNE_API UInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template<typename ResultType = UInventoryItemTrait>
	static ResultType* FindItemDefinitionTrait(const TSubclassOf<UInventoryItemDefinition>& ItemDef, const TSubclassOf<UInventoryItemTrait>& TraitClass);
};

template <typename ResultType>
ResultType* UInventoryFunctionLibrary::FindItemDefinitionTrait(const TSubclassOf<UInventoryItemDefinition>& ItemDef,
	const TSubclassOf<UInventoryItemTrait>& TraitClass)
{
	static_assert(TIsDerivedFrom<ResultType, UInventoryItemTrait>::Value, "ResultType 必须是 UInventoryItemTrait 子类");

	if (ItemDef == nullptr || TraitClass == nullptr)
	{
		return nullptr;
	}

	UInventoryItemTrait* FoundTrait = GetDefault<UInventoryItemDefinition>(ItemDef)->FindTraitByClass(TraitClass);
	return Cast<ResultType>(FoundTrait);
}
