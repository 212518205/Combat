// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "MenuEntryMapping.Generated.h"

/**
 * 
 */

class UMenuEntryBase;

UCLASS()
class KITSUNE_API UMenuEntryMapping : public UDataAsset
{
	GENERATED_BODY()

public:
	TSubclassOf<UMenuEntryBase> FindEntryClassByEntryType(const ETileViewEntryType& InEntryType) const;
protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<ETileViewEntryType, TSubclassOf<UMenuEntryBase>> TileEntryClassMapping;
};
