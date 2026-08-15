// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/KitsuneExtensionComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "UI/DataObjects/Inventory/InventorySlotData.h"
#include "InventorySystemComponent.generated.h"

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
class KITSUNE_API UInventorySystemComponent : public UKitsuneExtensionComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChanged, UInventoryItemInstance*, ChangedInstance,
	                                             EInstanceModifyType, ModifyType);
	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemChanged ItemChanged;

	/***  默认堆叠一个   `BC@` ***/
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void AddItem(UInventoryItemInstance* InItemInstance);

	TArray<TPair<FName, FInventoryCategoryGroup>> GetAllCategoryItem();
	TArray<UInventorySlotData*> GetAllItemsByCategory(const FName CategoryID);

	/***  运行时控制台命令函数   `BC@` ***/
	UFUNCTION(Exec)
	void DebugPrintInventory();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FInventoryItemArray InventoryItems;

};
