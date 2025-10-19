// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "EnemyCharacter.generated.h"

class UDataAssetStartDataEnemy;
/**
 * 
 */
UCLASS()
class KITSUNE_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	/***   ...ACharacterBase Interface Begin...   ***/
	virtual void InitAbilityInfo() override;
	virtual void BeginPlay() override;
	/***   ...ACharacterBase Interface End...     ***/

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UDataAssetStartDataEnemy> StartData;
};
