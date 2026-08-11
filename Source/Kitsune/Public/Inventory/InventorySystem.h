// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemInstance.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "InventorySystem.generated.h"

class UInventoryItemInstance;

// InventorySlotData.h
UCLASS(BlueprintType)
class KITSUNE_API UInventorySlotData : public UObject
{
	GENERATED_BODY()
public:
	UInventorySlotData(){}
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryItemInstance> ItemInstance = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bIsLocked = false;

	/*UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex = -1;*/

	UPROPERTY(BlueprintReadOnly)
	int32 UnlockCost = 0;
};




USTRUCT()
struct FInventoryCategoryGroup
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FText CategoryDisplayName;
	
	UPROPERTY()
	TArray<UInventorySlotData*> CategorySlots;
};


/**
 * 
 */
UCLASS()
class KITSUNE_API UInventorySystem : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChanged, UInventoryItemInstance*, ChangedInstance,
	                                             EInstanceModifyType, ModifyType);
	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemChanged ItemChanged;

	/***  默认堆叠一个，在合并情况下将InStackCount参数置为 0   `BC@` ***/
	bool AddItem(UInventoryItemInstance* InItemInstance, const int32 InStackCount = 1);

	TArray<TPair<FName, FInventoryCategoryGroup>> GetAllCategoryItem();
	TArray<UInventorySlotData*> GetAllItemsByCategory(const FName CategoryID);

	/***  运行时控制台命令函数   `BC@` ***/
	UFUNCTION(Exec)
	void DebugPrintInventory();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<UInventoryItemInstance*> InventoryItems;

};
