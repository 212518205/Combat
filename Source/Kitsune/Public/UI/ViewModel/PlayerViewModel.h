// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/InventorySystemComponent.h"
#include "UI/ViewModel/AttributeViewModel.h"
#include "PlayerViewModel.generated.h"

class UListDataObjectString;

UENUM()
enum class EItemInstanceAction : uint8
{
	EAddInstance = 1,
	ERemoveInstance = 2,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractableItemChange, UInventoryItemInstance*, ItemInstance, EItemInstanceAction, InstanceAction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerCategoryCapacityChanged, FName, CategoryID, int32, CategoryCapacity);

/**
 * 
 */
UCLASS()
class KITSUNE_API UPlayerViewModel : public UAttributeViewModel
{
	GENERATED_BODY()

public:
	virtual void NativeInitialize() override;
	void AddInteractableItemInstance(UInventoryItemInstance* ItemInstance);
	void RemoveInteractableItemInstance(UInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable)
	void SetPlayerWeaponIcon(TSoftObjectPtr<UTexture2D> InWeaponIcon);

	/***  变化委托   `BC@` ***/
	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnAttributeChangedDelegate OnWeaponChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnPlayerCategoryCapacityChanged CategoryCapacityChanged;

	UPROPERTY(BlueprintAssignable, Category  = "ViewModel | Interact")
	FOnInteractableItemChange OnInteractableItemChange;

	/***  Getter   `BC@` ***/
	TArray<UInventoryItemInstance*>& GetOverlappedItemInstances() { return OverlappedItemInstances; }   // 与所属玩家重叠的物品
	TArray<TPair<FName, FInventoryCategoryGroup>> GetAllInventoryCategories() const;				    // 获取所有库存物品-按分类
	TArray<UInventorySlotData*> GetInventoryItemsByCategory(const FName CategoryID) const;			    // 获取对应分类的物品
	int32 GetCategoryCapacity(const FName CategoryID) const;											// 获取对应分类的格子容量

	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | WeaponIcon")
	TSoftObjectPtr<UTexture2D> WeaponIcon;
	
protected:
	UFUNCTION()
	void OnCategoryCapacityChanged(FName CategoryID, int32 CategoryCapacity);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventorySystemComponent> CarriedInventorySystem;
	
	UPROPERTY(BlueprintReadWrite, Category = "ViewModel | Interact")
	TArray<UInventoryItemInstance*> OverlappedItemInstances;
};
