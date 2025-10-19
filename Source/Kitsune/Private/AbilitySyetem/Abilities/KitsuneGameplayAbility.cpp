﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySyetem/Abilities/KitsuneGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Chaos/Deformable/MuscleActivationConstraints.h"
#include "Component/Combat/KitsuneCombatComponent.h"
#include "FunctionLibrary/KitsuneFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"

UKitsuneCombatComponent* UKitsuneGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UKitsuneCombatComponent>();
}

void UKitsuneGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (ActivationPolicy==EKitsuneAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo&&!Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UKitsuneGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (ActivationPolicy==EKitsuneAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

FGameplayEffectSpecHandle UKitsuneGameplayAbility::MakeWeaponDamageEffectSpecHandle(
	const TSubclassOf<UGameplayEffect> EffectClass, const FGameplayTag AttackType, const int32 WeaponAttackCount) const
{
	check(EffectClass);

	const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	EffectContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		EffectContextHandle
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(AttackType, WeaponAttackCount);
	return EffectSpecHandle;
}

FActiveGameplayEffectHandle UKitsuneGameplayAbility::NativeApplyGameplayEffectSpecToTarget(
	const FGameplayEffectSpecHandle& SpecHandle,
	AActor* TargetActor)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	check(TargetASC && SpecHandle.IsValid())

	return TargetASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetASC);
}

FActiveGameplayEffectHandle UKitsuneGameplayAbility::BP_ApplyGameplayEffectSpecToTarget(
	const FGameplayEffectSpecHandle& SpecHandle, AActor* TargetActor, EKitsuneSuccessType& ApplySuccessType)
{
	FActiveGameplayEffectHandle ActiveEffectHandle = NativeApplyGameplayEffectSpecToTarget(SpecHandle, TargetActor);

	ApplySuccessType = ActiveEffectHandle.IsValid() ? EKitsuneSuccessType::Successful : EKitsuneSuccessType::Failed;

	return ActiveEffectHandle;
}


