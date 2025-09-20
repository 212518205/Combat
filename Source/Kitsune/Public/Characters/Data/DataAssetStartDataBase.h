// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAssetStartDataBase.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class KITSUNE_API UDataAssetStartDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	/*** `@BC`   Description: 初始化初始能力   `BC@` ***/
	void InitializeForASC(UAbilitySystemComponent* InTargetASC, int32 AbilityLevel);

protected:
	/*** `@BC`   Description: 初始能力   `BC@` ***/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Ability")
	TArray<TSubclassOf<UGameplayAbility>> GiveAndActiveAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TArray<TSubclassOf<UGameplayAbility>> ReactivableAbilities;

	static void GrantAbilitiesToASC(UAbilitySystemComponent* TargetASC,
	                                TArray<TSubclassOf<UGameplayAbility>>& UnassignedAbilities, int32 AbilityLevel);

	/*** `@BC`   Description: 初始属性   `BC@` ***/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Effect")
	TArray<TSubclassOf<UGameplayEffect>> DefaultGameplayEffects;

	static void ApplyGameplayEffectToASC(UAbilitySystemComponent* TargetASC,
		TArray<TSubclassOf<UGameplayEffect>>& UnassignedEffect, int32 EffectLevel);
};
