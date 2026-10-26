// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/KitsuneToggleAbility.h"
#include "AbilitySystemComponent.h"

void UKitsuneToggleAbility::InputPressed(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UKitsuneToggleAbility::IsToggleOn() const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	return ASC && ToggleStateTag.IsValid() && ASC->HasMatchingGameplayTag(ToggleStateTag);
}

void UKitsuneToggleAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsActive())
	{
		return;
	}

	ensureAlwaysMsgf(ToggleStateTag.IsValid(),
		TEXT("%s 未配置 ToggleStateTag：IsToggleOn() 将恒为 false，K2_OnToggleOff 不会触发"), *GetName());

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (ToggleStateTag.IsValid())
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->AddReplicatedLooseGameplayTag(ToggleStateTag);
		}
	}

	OnToggleOn();
}

void UKitsuneToggleAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo,
                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                        bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsToggleOn())
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveReplicatedLooseGameplayTag(ToggleStateTag);
		}

		OnToggleOff();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UKitsuneToggleAbility::OnToggleOn()
{
	K2_OnToggleOn();
}

void UKitsuneToggleAbility::OnToggleOff()
{
	K2_OnToggleOff();
}
