// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"AbilitySystemInterface.h"
#include "CharacterBase.generated.h"

class UPlayerCombatComponent;
class UDataAssetStartDataBase;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class KITSUNE_API ACharacterBase : public ACharacter , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	/***   ...IAbilitySystemInterface Interface Begin...   ***/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/***   ...IAbilitySystemInterface Interface End...     ***/
	UAttributeSet* GetAttributeSet() const;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityInfo();

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	/*** `@BC`   Description: 初始化角色能力、属性等信息   `BC@` ***/
	UPROPERTY(EditDefaultsOnly, Category = "Initial Info")
	TObjectPtr<UDataAssetStartDataBase> InitialInfoData;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Initial Info")
	int32 CharacterLevel = 1;
};
