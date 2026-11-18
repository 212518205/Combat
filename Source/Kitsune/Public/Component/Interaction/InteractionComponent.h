
#pragma once

#include "CoreMinimal.h"
#include "Component/KitsuneExtensionComponent.h"
// [VMREFACTOR] EItemInstanceAction 随 UInteractionViewModel 一起迁走，这里只为 RPC 参数声明需要
#include "UI/ViewModel/InteractionViewModel.h"
#include "InteractionComponent.generated.h"

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
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void EnsureInteractionViewModel();

protected:

	UFUNCTION(Client, Reliable)
	void ItemOverlapChange(UInventoryItemInstance* ItemInstance, EItemInstanceAction InstanceAction);

};
