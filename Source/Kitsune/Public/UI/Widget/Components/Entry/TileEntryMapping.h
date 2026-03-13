// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "TileEntryMapping.Generated.h"

/**
 * 
 */

class UTileViewEntryBase;

UCLASS()
class KITSUNE_API UTileEntryMapping : public UDataAsset
{
	GENERATED_BODY()

public:
	TSubclassOf<UTileViewEntryBase> FindEntryClassByEntryType(const ETileViewEntryType& InEntryType) const;
protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<ETileViewEntryType, TSubclassOf<UTileViewEntryBase>> TileEntryClassMapping;
};
