// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KitsuneInventoryFunctionLibrary.generated.h"

UCLASS()
class KITSUNE_API UInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template<typename ResultType = UInventoryItemTrait>
	static ResultType* FindItemDefinitionTrait(const UInventoryItemDefinition* ItemDef);
};

#define GET_TRAIT(ItemInstance, TraitSuffix) \
		UInventoryFunctionLibrary::FindItemDefinitionTrait<UInventoryItemTrait_##TraitSuffix>((ItemInstance)->GetItemDef())

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
