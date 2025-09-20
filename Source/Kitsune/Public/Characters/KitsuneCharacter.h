// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Input/KitsuneInputConfig.h"
#include "KitsuneCharacter.generated.h"


class UPlayerCombatComponent;
/**
 * 
 */
UCLASS(Abstract)
class KITSUNE_API AKitsuneCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AKitsuneCharacter();
	/***   ...ACharacterBase Interface Begin...   ***/
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	/***   ...ACharacterBase Interface End...     ***/

	/*** `@BC`   描述: Getter 函数   `BC@` ***/
	UFUNCTION(BlueprintCallable, Category = "Combat")
	UPlayerCombatComponent* GetCombatComponent();

protected:
	/*** `@BC`   描述: 初始化Gameplay框架相关信息   `BC@` ***/
	virtual void InitAbilityInfo() override;

	/*** `@BC`   描述: 角色扩展组件   `BC@` ***/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component|Combat")
	TObjectPtr<UPlayerCombatComponent> CombatComponent;
};
