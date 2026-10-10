// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ListEntryMapping.generated.h"

class UInventoryItemInstance;
class UInventoryItemTrait;
class UListEntryInteract;
class UListDataObjectBase;
class UListEntryBase;
/**
 * 
 */
UCLASS()
class KITSUNE_API UListEntryMapping : public UDataAsset
{
	GENERATED_BODY()

public:
	TSubclassOf<UListEntryBase> FindEntryClassByDataObject(const UListDataObjectBase* InListDataObject) const;

	TSubclassOf<UListEntryBase> FindEntryClassByItemTrait(const UInventoryItemTrait* InItemTrait) const;
protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UListDataObjectBase>, TSubclassOf<UListEntryBase>> ListEntryClassMapping;

	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UInventoryItemTrait>, TSubclassOf<UListEntryBase>> InteractListEntryMapping;
};
