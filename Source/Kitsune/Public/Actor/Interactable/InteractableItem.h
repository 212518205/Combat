// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/ItemBase.h"
#include "InteractableItem.generated.h"

class UBoxComponent;
class UInventoryItemInstance;
/**
 * 
 */
UCLASS()
class KITSUNE_API AInteractableItem : public AItemBase
{
	GENERATED_BODY()

public:
	AInteractableItem();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Instanced, Category = "Inventory")
	TObjectPtr<UInventoryItemInstance> ItemInstance;

	UFUNCTION()
	void OnBoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
