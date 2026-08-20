// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KitsuneGameplayAbility.h"
#include "ActiveGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UActiveGameplayAbility : public UKitsuneGameplayAbility
{
	GENERATED_BODY()
	
public:
	FKey GetTriggerKey() const { return TriggerKey; }
	FString GetDisplayName() const { return DisplayName; }
	UTexture2D* GetAbilityIcon() const { return AbilityIcon; }
	int GetPriority() const { return Priority; }
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info")
	FKey TriggerKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info")
	TObjectPtr<UTexture2D> AbilityIcon;
	
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info")
	FString DisplayName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info")
	int Priority = 0;
	
};
