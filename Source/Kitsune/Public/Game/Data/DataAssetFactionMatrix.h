// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "DataAssetFactionMatrix.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UDataAssetFactionMatrix : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FFactionPreset Preset;
	
};
