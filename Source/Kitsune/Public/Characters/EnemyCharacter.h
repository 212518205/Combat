// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Interfaces/LockableInterface.h"
#include "EnemyCharacter.generated.h"

// [VMREFACTOR] UEnemyViewModel 已删除：敌人和玩家共用 UASCViewModel，统一走 Registry
class UASCViewModel;
class UEnemyKitsuneCombatComponent;
class UDataAssetStartDataEnemy;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class KITSUNE_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	virtual void BeginPlay() override;
	
	/***   ...IPawnCombatInterface Interface Begin...   ***/
	virtual UKitsuneCombatComponent* GetKitsuneCombatComponent() const override;
	/***   ...IPawnCombatInterface Interface End...     ***/

protected:
	/***   ...ACharacterBase Interface Begin...   ***/
	virtual void InitAbilityInfo() override;
	/***   ...ACharacterBase Interface End...     ***/

	// [VMREFACTOR] 保留函数名供蓝图调用，返回类型改为 UASCViewModel（蓝图节点需重新指向）
	UFUNCTION(BlueprintCallable, Category = "UI | ViewModel")
	UASCViewModel* GetEnemyViewModel();

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UDataAssetStartDataEnemy> StartData;

	/*** `@BC`   描述: 角色扩展组件   `BC@` ***/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component | Combat")
	TObjectPtr<UEnemyKitsuneCombatComponent> CombatComponent;

	// [VMREFACTOR] 删除 UEnemyViewModel* EnemyViewModel 成员：不再有"组件/角色各存一份"的双份所有权
};
