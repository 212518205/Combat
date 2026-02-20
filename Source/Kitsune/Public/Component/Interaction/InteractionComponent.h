// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/KitsuneExtensionComponent.h"
#include "InteractionComponent.generated.h"

class UPlayerViewModel;
class UInventoryItemInstance;
/**
 * 
 */
UCLASS()
class KITSUNE_API UInteractionComponent : public UKitsuneExtensionComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	UFUNCTION(Server, Reliable)
	void AddInteractableItem(UInventoryItemInstance* ItemInstance);

	UFUNCTION(Server, Reliable)
	void RemoveInteractableItem(UInventoryItemInstance* ItemInstance);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Client, Reliable)
	void ItemOverlapChange(UInventoryItemInstance* ItemInstance, EItemInstanceAction InstanceAction);

	UPlayerViewModel* GetLocalPlayerViewModel();

	TWeakObjectPtr<UPlayerViewModel> CachedLocalViewModel;
};


