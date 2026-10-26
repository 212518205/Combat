// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/KitsuneChanneledAbility.h"
#include "AbilitySystemComponent.h"

void UKitsuneChanneledAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (IsActive())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

float UKitsuneChanneledAbility::GetChannelTime() const
{
	if (!IsChanneling())
	{
		return 0.f;
	}

	const UWorld* World = GetWorld();
	return World ? static_cast<float>(World->GetTimeSeconds() - ChannelStartTime) : 0.f;
}

bool UKitsuneChanneledAbility::IsChanneling() const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	return ASC && ChannelStateTag.IsValid() && ASC->HasMatchingGameplayTag(ChannelStateTag);
}

void UKitsuneChanneledAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsActive())
	{
		return;
	}

	ensureAlwaysMsgf(ChannelStateTag.IsValid(),
		TEXT("%s 未配置 ChannelStateTag：IsChanneling() 将恒为 false，K2_OnChannelEnd 不会触发"), *GetName());

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const UWorld* World = GetWorld();
	ChannelStartTime = World ? World->GetTimeSeconds() : 0.0;

	if (ChannelStateTag.IsValid())
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->AddReplicatedLooseGameplayTag(ChannelStateTag);
		}
	}

	OnChannelBegin();
}

void UKitsuneChanneledAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsChanneling())
	{
		const float ChannelTime = GetChannelTime();

		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveReplicatedLooseGameplayTag(ChannelStateTag);
		}

		OnChannelEnd(ChannelTime);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UKitsuneChanneledAbility::OnChannelBegin()
{
	K2_OnChannelBegin();
}

void UKitsuneChanneledAbility::OnChannelEnd(float ChannelTime)
{
	K2_OnChannelEnd(ChannelTime);
}
