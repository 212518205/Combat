// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/KitsuneExtensionComponent.h"
#include "InteractionComponent.generated.h"

class UInventorySystem;
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
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void AddInteractableItem(UInventoryItemInstance* ItemInstance);

	UFUNCTION(Server, Reliable)
	void RemoveInteractableItem(UInventoryItemInstance* ItemInstance);
	
	UInventorySystem* GetInventorySystem();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UFUNCTION(Client, Reliable)
	void ItemOverlapChange(UInventoryItemInstance* ItemInstance, EItemInstanceAction InstanceAction);

	UPlayerViewModel* GetLocalPlayerViewModel();

	/*** TODO: 这里不应该缓存viewmodel，在交互组件处理，外部通过viewmodel访问交互组件... [2026年4月25日 14:29:38 来自`@BC@`] ***/
	TWeakObjectPtr<UPlayerViewModel> CachedLocalViewModel;
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Interact")
	UInventorySystem* InventorySystem;
};


