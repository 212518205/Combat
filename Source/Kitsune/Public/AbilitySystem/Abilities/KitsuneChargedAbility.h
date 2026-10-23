// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KitsuneInputAbility.h"
#include "KitsuneChargedAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class KITSUNE_API UKitsuneChargedAbility : public UKitsuneInputAbility
{
	GENERATED_BODY()
	
public:
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           const FGameplayAbilityActivationInfo ActivationInfo) override;

	UFUNCTION(BlueprintPure)
	float GetCurrentChargeTime() const;
	
	UFUNCTION(BlueprintPure)
	float GetChargePercent() const;
	
	UFUNCTION(BlueprintPure)
	bool IsCharging() const;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	
	virtual void OnChargeBegin();
	virtual void OnCharged(float ChargeTime, float ChargePercent);
	virtual void OnChargeCancelled();
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnChargeBegin();
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnCharged(float ChargeTime, float ChargePercent);
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnChargeCancelled();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitsune", meta = (ClampMin = "0.0"))
	float MinChargeTime = 0.15f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitsune", meta = (ClampMin = "0.0"))
	float MaxChargeTime = 0.2f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Shared.Status"))
	FGameplayTag ChargeStateTag;
	
private:
	double ChargeStartTime = 0.0;

	bool bCharging = false;

	bool bChargingTagApplied = false;

	bool bChargeResolved = false;
};
