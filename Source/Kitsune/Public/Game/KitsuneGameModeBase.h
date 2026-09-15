// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/DataAssetFactionMatrix.h"
#include "GameFramework/GameModeBase.h"
#include "KitsuneGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API AKitsuneGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UDataAssetFactionMatrix> FactionPreset;
	
};
