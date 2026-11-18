// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/KitsuneExtensionComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "CommonTypes/EnumTypes.h"
#include "CommonTypes/StructTypes.h"
#include "Interfaces/SavableInterface.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryComponent.generated.h"

struct FInventoryCategoryConfig;
class UInventoryItemInstance;

USTRUCT(BlueprintType)
struct FInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<UInventoryItemInstance> ItemInstance;
	
};

USTRUCT(BlueprintType)
struct FInventoryArray : public FFastArraySerializer
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
	TArray<FInventoryEntry> Items;
	
	UPROPERTY(Transient)
	TObjectPtr<UInventoryComponent> Owner{nullptr};
	
};

template<>
struct TStructOpsTypeTraits<FInventoryArray> : public TStructOpsTypeTraitsBase2<FInventoryArray>
{
	enum  { WithNetDeltaSerializer = true };
};

/**
 * 
 */
UCLASS()
class KITSUNE_API UInventoryComponent : public UKitsuneExtensionComponent, public ISavableInterface
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	/***   ...ISavableInterface Interface Begin...   ***/
	virtual void SaveTo(UKitsuneSaveGame* SaveGame) override;
	virtual void LoadFrom(const UKitsuneSaveGame* SaveGame) override;
	virtual int64 GetSavePlayerUID() override;
	/***   ...ISavableInterface Interface End...     ***/
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChanged, UInventoryItemInstance*, ChangedInstance,
	                                             EInventoryItemChangeType, ChangeType);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCategoryCapacityChanged, FName, CategoryID, int32, CategoryCapacity);
	
	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemChanged ItemChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnCategoryCapacityChanged CapacityChanged;

	TArray<UInventoryItemInstance*> GetAllItemInstances() const;

	/***  默认堆叠一个   `BC@` ***/
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void AddItem(UInventoryItemInstance* InItemInstance);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void UnlockCategorySlots(const FName CategoryID);

	int32 GetCapacityByCategoryID(const FName CategoryID) const;

	/** [VMREFACTOR] 幂等：BeginPlay 时 Pawn 可能还没被 Possess（IsLocallyControlled 为假），
	 * [VMREFACTOR] 所以角色在 InitAbilityInfo 里会再调一次这里 */
	void EnsureInventoryViewModel();
	

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FInventoryArray InventoryItems;
	
	UPROPERTY(ReplicatedUsing = OnRep_UnlockedByCategory, BlueprintReadOnly)
	TArray<FCategoryCapacityEntry> UnlockedByCategory;

	UFUNCTION()
	void OnRep_UnlockedByCategory(const TArray<FCategoryCapacityEntry>& OldValue);

private:
	static const FInventoryCategoryConfig* GetCategoryInfo(const FName CategoryID);
	bool TrySpendGold(int32 Cost);
	
};
