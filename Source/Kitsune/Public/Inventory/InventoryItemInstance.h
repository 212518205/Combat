// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTypes/EnumTypes.h"
#include "CommonTypes/StructTypes.h"
#include "InventoryItemInstance.generated.h"

class UInventoryComponent;
class UInventoryItemDefinition;


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class KITSUNE_API UInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	friend  UInventoryComponent;

	UInventoryItemInstance();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	
	UInventoryItemInstance* CreateInstanceCopy(UObject* NewOuter)const;

	FORCEINLINE UInventoryItemDefinition* GetItemDef() const { return ItemDef; }
	FORCEINLINE AActor* GetOwningActor() const {return OwningActor;}
	FORCEINLINE void SetOwningActor(AActor* Actor) {OwningActor = Actor;}

	/*** `@BC`   描述: EInventoryItemFeature操作函数   `BC@`(ItemFeatures & Feature) != EInventoryItemFeature::None;  ***/
	FORCEINLINE void AddFeature(const EInventoryItemFeature Feature) { ItemFeatures |= static_cast<int32>(Feature); }
	FORCEINLINE void RemoveFeature(const EInventoryItemFeature Feature) { ItemFeatures &= ~static_cast<int32>(Feature); }
	FORCEINLINE bool HasFeature(const EInventoryItemFeature Feature) const { return (ItemFeatures & static_cast<int32>(Feature)) == static_cast<int32>(Feature); }
	FORCEINLINE bool HasAnyFeature(const int32 Feature) const {
		return (ItemFeatures & Feature) != 0;
	}
	FORCEINLINE void ClearAllFeatures() { ItemFeatures = 0; }
	FORCEINLINE void ToggleFeature(const EInventoryItemFeature Feature)
	{
		if (HasFeature(Feature))RemoveFeature(Feature);
		else AddFeature(Feature);
	}	
	
	int32 GetStackCount() const { return StackCount; }
	FORCEINLINE int32 GetItemFeature() const { return ItemFeatures; }
	FSerializedItem ToSerialized() const;
	static UInventoryItemInstance* CreateFromSerialized(UObject* NewOuter, const FSerializedItem& Serialized);

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 StackCount = 1;

	UPROPERTY(Replicated, EditDefaultsOnly)
	TObjectPtr<UInventoryItemDefinition> ItemDef;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, meta = (Bitmask, BitmaskEnum = "/Script/Kitsune.EInventoryItemFeature"))
	int32 ItemFeatures = 0;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> OwningActor;

private:
	static UInventoryItemDefinition* ResolveItemDef(const FSerializedItem& Serialized);
	
};
