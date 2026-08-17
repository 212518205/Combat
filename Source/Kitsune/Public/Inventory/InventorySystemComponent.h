// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/KitsuneExtensionComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "Interfaces/SavableInterface.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "UI/DataObjects/Inventory/InventorySlotData.h"
#include "InventorySystemComponent.generated.h"

struct FInventoryInfo;
class UInventoryItemInstance;

USTRUCT(BlueprintType)
struct FInventoryItemEntry: public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<UInventoryItemInstance> ItemInstance;
	
};

USTRUCT(BlueprintType)
struct FInventoryItemArray: public FFastArraySerializer
{
	GENERATED_BODY()
	
public:
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& Params)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize(Items, Params, *this);
	}
	
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	void PreReplicatedRemove(const TArrayView<int32> RemoveIndices, int32 FinalSize);
	
	UPROPERTY()
	TArray<FInventoryItemEntry> Items;
	
	UPROPERTY(Transient)
	TObjectPtr<UInventorySystemComponent> Owner{nullptr};
	
};

template<>
struct TStructOpsTypeTraits<FInventoryItemArray> : public TStructOpsTypeTraitsBase2<FInventoryItemArray>
{
	enum  { WithNetDeltaSerializer = true };
};

/**
 * 
 */
UCLASS()
class KITSUNE_API UInventorySystemComponent : public UKitsuneExtensionComponent, public ISavableInterface
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChanged, UInventoryItemInstance*, ChangedInstance,
	                                             EInstanceModifyType, ModifyType);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCategoryCapacityChanged);
	
	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemChanged ItemChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnCategoryCapacityChanged CapacityChanged;

	/***   ...ISavableInterface Interface Begin...   ***/
	virtual void SaveTo(UKitsuneSaveGame* SaveGame) override;
	virtual void LoadFrom(const UKitsuneSaveGame* SaveGame) override;
	/***   ...ISavableInterface Interface End...     ***/

	/***  默认堆叠一个   `BC@` ***/
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void AddItem(UInventoryItemInstance* InItemInstance);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void UnlockCategorySlots(const FName CategoryID);
	
	int32 GetCapacityByCategoryID(const FName CategoryID);

	TArray<TPair<FName, FInventoryCategoryGroup>> GetAllCategoryItem();
	TArray<UInventorySlotData*> GetAllItemsByCategory(const FName CategoryID);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FInventoryItemArray InventoryItems;
	
	UPROPERTY(ReplicatedUsing = OnRep_InventoryCapacity, BlueprintReadOnly)
	TArray<FCategoryCapacityEntry> InventoryCapacity;
	
	UFUNCTION()
	void OnRep_InventoryCapacity();

private:
	static const FInventoryInfo* GetCategoryInfo(const FName CategoryID);
	bool TrySpendGold(int32 Cost);
	
};
