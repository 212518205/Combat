// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CommonTypes/EnumTypes.h"
#include "MenuEntryMapping.generated.h"

/**
 * 
 */

class UMenuEntryBase;

UCLASS()
class KITSUNE_API UMenuEntryMapping : public UDataAsset
{
	GENERATED_BODY()

public:
	TSubclassOf<UMenuEntryBase> FindEntryClassByEntryType(const EListEntryIconStyle& InEntryType) const;
protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<EListEntryIconStyle, TSubclassOf<UMenuEntryBase>> TileEntryClassMapping;
};
