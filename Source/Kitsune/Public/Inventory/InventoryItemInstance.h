// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemInstance.generated.h"

class UInventoryItemDefinition;


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class KITSUNE_API UInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UInventoryItemInstance();

	FORCEINLINE TSubclassOf<UInventoryItemDefinition> GetItemDef() const { return ItemDef; }
	FORCEINLINE FGuid GetInstanceID() const { return InstanceID; }

	/*** `@BC`   描述: EItemFeature操作函数   `BC@`(ItemFeatures & Feature) != EItemFeature::None;  ***/
	//FORCEINLINE void AddFeature(const EItemFeature Feature) { ItemFeatures |= Feature; }
	//FORCEINLINE void RemoveFeature(const EItemFeature Feature) { ItemFeatures &= ~Feature; }
	//FORCEINLINE bool HasFeature(const EItemFeature Feature) const { return (ItemFeatures & Feature) == Feature; }
	FORCEINLINE bool HasAnyFeature(const EItemFeature Feature) const {
		return (static_cast<uint8>(ItemFeatures) & static_cast<uint8>(Feature)) != 0;
	}
	/*FORCEINLINE void ClearAllFeatures() { ItemFeatures = EItemFeature::None; }
	FORCEINLINE void ToggleFeature(const EItemFeature Feature)
	{
		if (HasFeature(Feature))RemoveFeature(Feature);
		else AddFeature(Feature);
	}*/

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UInventoryItemDefinition> ItemDef;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, meta = (Bitmask, BitmaskEnum = EItemFeature))
	uint8 ItemFeatures;

	UPROPERTY(BlueprintReadOnly)
	FGuid InstanceID;


};
