// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "InventorySystem.generated.h"

class UInventoryItemInstance;
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

	/***  默认堆叠一个，在合并情况下将InStackCount参数置0   `BC@` ***/
	bool AddItem(UInventoryItemInstance* InItemInstance, const int32 InStackCount = 1);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/***  运行时控制台命令函数   `BC@` ***/
	UFUNCTION(Exec)
	void DebugPrintInventory();

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<UInventoryItemInstance*> InventoryItems;

};
