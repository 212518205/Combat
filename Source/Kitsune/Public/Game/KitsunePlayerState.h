// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include"AbilitySystemInterface.h"
#include "KitsunePlayerState.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class KITSUNE_API AKitsunePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AKitsunePlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;
	
	int64 GetPlayerUID() const { return PlayerUID; }
	void SetPlayerUID(const int64 InUID) { PlayerUID = InUID; }
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(ReplicatedUsing = OnRep_PlayerUID, BlueprintReadOnly)
	int64 PlayerUID = 0;
	
	UFUNCTION()
	void OnRep_PlayerUID();
	
};
