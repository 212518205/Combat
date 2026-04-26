// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendDebugHelper.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemInstance.generated.h"

class UInventorySystem;
class UInventoryItemDefinition;


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class KITSUNE_API UInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	friend  UInventorySystem;

	UInventoryItemInstance();

	FORCEINLINE UInventoryItemDefinition* GetItemDef() const { return ItemDef; }
	FORCEINLINE AActor* GetOwningActor() const {return OwningActor;}
	FORCEINLINE void SetOwningActor(AActor* Actor) {OwningActor = Actor;}

	/*** `@BC`   描述: EItemFeature操作函数   `BC@`(ItemFeatures & Feature) != EItemFeature::None;  ***/
	FORCEINLINE void AddFeature(const EItemFeature Feature) { ItemFeatures |= static_cast<int32>(Feature); }
	FORCEINLINE void RemoveFeature(const EItemFeature Feature) { ItemFeatures &= ~static_cast<int32>(Feature); }
	FORCEINLINE bool HasFeature(const EItemFeature Feature) const { return (ItemFeatures & static_cast<int32>(Feature)) == static_cast<int32>(Feature); }
	FORCEINLINE bool HasAnyFeature(UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/PoseSearch.EPoseSearchBoneFlags")) const int32 Feature) const {
		return (ItemFeatures & Feature) != 0;
	}
	FORCEINLINE void ClearAllFeatures() { ItemFeatures = 0; }
	FORCEINLINE void ToggleFeature(const EItemFeature Feature)
	{
		if (HasFeature(Feature))RemoveFeature(Feature);
		else AddFeature(Feature);
	}	
	
	UPROPERTY(BlueprintReadOnly)
	int32 StackCount = 1;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, EditDefaultsOnly, Instanced)
	TObjectPtr<UInventoryItemDefinition> ItemDef;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, meta = (Bitmask, BitmaskEnum = "/Script/Kitsune.EItemFeature"))
	int32 ItemFeatures = 0;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> OwningActor;
};
