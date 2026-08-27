// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Interfaces/LockableInterface.h"
#include "EnemyCharacter.generated.h"

class UEnemyViewModel;
class UEnemyKitsuneCombatComponent;
class UDataAssetStartDataEnemy;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class KITSUNE_API AEnemyCharacter : public ACharacterBase, public ILockableInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	virtual void BeginPlay() override;
	
	/***   ...IPawnCombatInterface Interface Begin...   ***/
	virtual UKitsuneCombatComponent* GetKitsuneCombatComponent() const override;
	/***   ...IPawnCombatInterface Interface End...     ***/

	/***   ...ILockableInterface Interface Begin...   ***/
	virtual void SetLockMarkerVisible(const bool bVisible) override;
	/***   ...ILockableInterface Interface End...     ***/

protected:
	/***   ...ACharacterBase Interface Begin...   ***/
	virtual void InitAbilityInfo() override;
	/***   ...ACharacterBase Interface End...     ***/


	UFUNCTION(BlueprintCallable, Category = "UI | ViewModel")
	UEnemyViewModel* GetEnemyViewModel();

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UDataAssetStartDataEnemy> StartData;

	/*** `@BC`   描述: 角色扩展组件   `BC@` ***/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component | Combat")
	TObjectPtr<UEnemyKitsuneCombatComponent> CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component | UI")
	TObjectPtr<UWidgetComponent> EnemyWidgetComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component | UI")
	TObjectPtr<UWidgetComponent> LockMarkerComponent;

	UPROPERTY(BlueprintReadOnly)
	UEnemyViewModel* EnemyViewModel;
	
	UPROPERTY(EditDefaultsOnly)
	FName LockMarkerSocketName;
};
