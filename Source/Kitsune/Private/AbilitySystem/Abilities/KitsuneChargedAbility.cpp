// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/KitsuneChargedAbility.h"

#include "AbilitySystemComponent.h"

void UKitsuneChargedAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (!IsCharging())
	{
		return;
	}

	const float CurrentChargeTime = GetCurrentChargeTime();
	const float ChargePercent = GetChargePercent();

	if (CurrentChargeTime < MinChargeTime || !CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		bChargeResolved = true;
		OnChargeCancelled();
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	bChargeResolved = true;
	OnCharged(CurrentChargeTime, ChargePercent);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

float UKitsuneChargedAbility::GetCurrentChargeTime() const
{
	if (!IsCharging())
	{
		return 0.f;
	}
	
	const UWorld* World = GetWorld();
	return World ? static_cast<float>(World->GetTimeSeconds() - ChargeStartTime) : 0.f;
}

float UKitsuneChargedAbility::GetChargePercent() const
{
	if (MaxChargeTime <= KINDA_SMALL_NUMBER)
	{
		return 1.f;
	}
	return FMath::Clamp(GetCurrentChargeTime() / MaxChargeTime, 0.f, 1.f);
}

bool UKitsuneChargedAbility::IsCharging() const
{
	return bCharging;
}

void UKitsuneChargedAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsActive())
	{
		return;
	}
	
	if (!CheckCost(Handle, ActorInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	const UWorld* World = GetWorld();
	ChargeStartTime = World ? World->GetTimeSeconds() : 0.f;
	bCharging = true;
	bChargeResolved = false;
	
	if (ChargeStateTag.IsValid())
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->AddReplicatedLooseGameplayTag(ChargeStateTag);
		}
	}
	OnChargeBegin();
}

void UKitsuneChargedAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!bChargeResolved && bCharging)
	{
		bChargeResolved = true;
		OnChargeCancelled();
	}

	if (ChargeStateTag.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveReplicatedLooseGameplayTag(ChargeStateTag);
	}
	
	bCharging = false;
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UKitsuneChargedAbility::OnChargeBegin()
{
	K2_OnChargeBegin();
}

void UKitsuneChargedAbility::OnCharged(float ChargeTime, float ChargePercent)
{
	K2_OnCharged(ChargeTime, ChargePercent);
}

void UKitsuneChargedAbility::OnChargeCancelled()
{
	K2_OnChargeCancelled();
}
