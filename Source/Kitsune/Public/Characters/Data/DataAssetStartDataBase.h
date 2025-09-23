// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DataAssetStartDataBase.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "InputTag"))
	FGameplayTag AbilityTag = FGameplayTag();
};

/**
 *
 */
UCLASS()
class KITSUNE_API UDataAssetStartDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	/*** `@BC`   Description: 初始化初始能力   `BC@` ***/
	virtual void InitAbilityAndEffectToASC(UAbilitySystemComponent* InTargetASC, int32 AbilityLevel);

protected:
	/*** `@BC`   Description: 初始能力   `BC@` ***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (ForceInlineRow, Categories = "InputTag.Weapon"))
	TArray<FAbilityInfo> OwningAbilities;

	static void GrantAbilitiesToASC(UAbilitySystemComponent* TargetASC,TArray<FAbilityInfo>& UnassignedAbilities, int32 AbilityLevel);
	/***  注释截止  ***/

	/*** `@BC`   Description: 初始属性   `BC@` ***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TArray<TSubclassOf<UGameplayEffect>> DefaultGameplayEffects;

	static void ApplyGameplayEffectToASC(UAbilitySystemComponent* TargetASC,TArray<TSubclassOf<UGameplayEffect>>& UnassignedEffect, int32 EffectLevel);
	/***  注释截止  ***/

	/*** TODO: 添加一些所属Actor对ASC所属Actor将会造成的影响. [2025年9月23日 15:45:01 来自`@BC@`] ***/

	/*** `@BC`   描述: 依据此父标签移除此在ASC中的能力   `BC@` ***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (Categories = "InputTag.Weapon"))
	FGameplayTag AbilityParentTag = FGameplayTag();
	/***  注释截止  ***/
};
