// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KitsuneGameplayAbility.h"
#include "Component/Combat/PlayerCombatComponent.h"
#include "ActiveGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UActiveGameplayAbility : public UKitsuneGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual UPlayerCombatComponent* GetPawnCombatComponentFromActorInfo() const override;
	UMotionWarpingComponent* GetMotionWarpingComponentFromActorInfo() const;
	
	UTexture2D* GetAbilityTriggerKeyIcon() const { return AbilityTriggerKeyIcon; }
	FString GetDisplayName() const { return DisplayName; }
	UTexture2D* GetAbilityIcon() const { return AbilityIcon; }
	FGameplayTag GetCooldownTag() const { return CooldownTag; }
	int GetPriority() const { return Priority; }
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info")
	TObjectPtr<UTexture2D> AbilityTriggerKeyIcon;
	
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info")
	TObjectPtr<UTexture2D> AbilityIcon;
	
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info")
	FString DisplayName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info")
	int Priority = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info", meta = (Categories = "Player.AbilityCooldown"))
	FGameplayTag CooldownTag = FGameplayTag::EmptyTag;
	
};
