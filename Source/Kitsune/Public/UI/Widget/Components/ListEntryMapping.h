// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ListEntryMapping.generated.h"

class UListDataObjectBase;
class UWidgetListEntryBase;
/**
 * 
 */
UCLASS()
class KITSUNE_API UListEntryMapping : public UDataAsset
{
	GENERATED_BODY()

public:
	TSubclassOf<UWidgetListEntryBase> FindEntryClassByDataObject(const UListDataObjectBase* InListDataObject) const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UListDataObjectBase>, TSubclassOf<UWidgetListEntryBase>> ListEntryClassMapping;
};
