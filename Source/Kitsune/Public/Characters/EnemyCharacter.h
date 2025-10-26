// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Interfaces/PawnCombatInterface.h"
#include "EnemyCharacter.generated.h"

class UEnemyKitsuneCombatComponent;
class UDataAssetStartDataEnemy;
/**
 * 
 */
UCLASS()
class KITSUNE_API AEnemyCharacter : public ACharacterBase, public IPawnCombatInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	/***   ...ACharacterBase Interface Begin...   ***/
	virtual void InitAbilityInfo() override;
	virtual void BeginPlay() override;
	/***   ...ACharacterBase Interface End...     ***/

	/***   ...IPawnCombatInterface Interface Begin...   ***/
	virtual UKitsuneCombatComponent* GetKitsuneCombatComponent() const override;
	/***   ...IPawnCombatInterface Interface End...     ***/

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UDataAssetStartDataEnemy> StartData;

	/*** `@BC`   描述: 角色扩展组件   `BC@` ***/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component|Combat")
	TObjectPtr<UEnemyKitsuneCombatComponent> CombatComponent;
};
