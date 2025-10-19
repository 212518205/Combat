// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "KitsuneGameplayAbility.generated.h"

class UKitsuneCombatComponent;

UENUM()
enum class EKitsuneAbilityActivationPolicy :uint8
{
	OnTriggered,
	OnGiven,
};

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/*** `@BC`   描述: 蓝图纯函数   `BC@` ***/
	UFUNCTION(BlueprintPure, Category = "Kitsune|Ability")
	UKitsuneCombatComponent* GetPawnCombatComponentFromActorInfo() const;

protected:
	/***   ...UGameplayAbility Interface Begin...   ***/
	/*** `@BC`   描述: 追加能力Give是否激活策略   `BC@` ***/
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	/***   ...UGameplayAbility Interface End...     ***/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitsune|Policy")
	EKitsuneAbilityActivationPolicy ActivationPolicy = EKitsuneAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "Kitsune|Ability")
	FGameplayEffectSpecHandle MakeWeaponDamageEffectSpecHandle(const TSubclassOf<UGameplayEffect> EffectClass,
		FGameplayTag AttackType,
		int32 WeaponAttackCount) const;

	static FActiveGameplayEffectHandle NativeApplyGameplayEffectSpecToTarget(const FGameplayEffectSpecHandle& SpecHandle,
	                                                                   AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Kitsune|Ability",
		meta = (DesplayName = "ApplyGameplayEffectSpecToTarget", ExpandEnumAsExecs = ApplySuccessType))
	static FActiveGameplayEffectHandle BP_ApplyGameplayEffectSpecToTarget(const FGameplayEffectSpecHandle& SpecHandle,
		AActor* TargetActor, EKitsuneSuccessType& ApplySuccessType);
};
