// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"AbilitySystemInterface.h"
#include "Data/DataAssetStartDataBase.h"
#include "Interfaces/LockableInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "CharacterBase.generated.h"

class UKitsuneWidgetComponent;
class UWidgetComponent;
class UMotionWarpingComponent;
class UPlayerCombatComponent;
class UDataAssetStartDataBase;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class KITSUNE_API ACharacterBase : public ACharacter , public IAbilitySystemInterface, public IPawnCombatInterface, public ILockableInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual void BeginPlay() override;

	/***   ...IAbilitySystemInterface Interface Begin...   ***/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/***   ...IAbilitySystemInterface Interface End...     ***/

	/***   ...IPawnCombatInterface Interface Begin...   ***/
	UFUNCTION(BlueprintCallable, Category = "Interface")
	virtual UKitsuneCombatComponent* GetKitsuneCombatComponent() const override { return nullptr; }
	/***   ...IPawnCombatInterface Interface End...     ***/
	
	/***   ...ILockableInterface Interface Begin...   ***/
	virtual void UpdateLockMarkerVisible(const bool bVisible) override;
	/***   ...ILockableInterface Interface End...     ***/

	UAttributeSet* GetAttributeSet() const;
	
	void SetCharacterProperties(const FCharacterProperties& CharacterProperties) const;
	UDataAssetStartDataBase* GetInitialInfoData() const { return InitialInfoData; }

protected:
	virtual void InitAbilityInfo();
	
	void OnWidgetComponentInitialized();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component | UI")
	TObjectPtr<UKitsuneWidgetComponent> LockMarkerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component | UI")
	TObjectPtr<UKitsuneWidgetComponent> CharacterStateComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component | UI")
	TObjectPtr<UKitsuneWidgetComponent> DamagePopupComp;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	/*** `@BC`   Description: 初始化角色能力、属性等信息   `BC@` ***/
	UPROPERTY(EditDefaultsOnly, Category = "Initial Info")
	TObjectPtr<UDataAssetStartDataBase> InitialInfoData;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Initial Info")
	int32 CharacterLevel = 1;
	
	UPROPERTY(EditDefaultsOnly)
	FName LockMarkerSocketName;
};
