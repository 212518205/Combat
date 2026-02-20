// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemInstance.h"
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

	//void RemoveOrAddInteractableItemInstance(UInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable)
	void SetPlayerWeaponIcon(TSoftObjectPtr<UTexture2D> InWeaponIcon);

	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnAttributeChangedDelegate OnWeaponChanged;

	UPROPERTY(BlueprintAssignable, Category  = "ViewModel | Interact")
	FOnInteractableItemChange OnInteractableItemChange;

	TArray<UInventoryItemInstance*>& GetOverlappedItemInstances() { return OverlappedItemInstances; }

	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | WeaponIcon")
	TSoftObjectPtr<UTexture2D> WeaponIcon;

	UPROPERTY(BlueprintReadWrite, Category = "ViewModel | Interact")
	TArray<UInventoryItemInstance*> OverlappedItemInstances;
};
