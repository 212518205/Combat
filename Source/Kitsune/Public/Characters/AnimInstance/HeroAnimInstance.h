// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/AnimInstance/KitsuneAnimInstance.h"
#include "HeroAnimInstance.generated.h"

class UCharacterMovementComponent;
class AKitsuneCharacter;
/**
 * 
 */
UCLASS()
class KITSUNE_API UHeroAnimInstance : public UKitsuneAnimInstance
{
	GENERATED_BODY()

public:
	/***   ...UKitsuneAnimInstance Interface Begin...   ***/
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	/***   ...UKitsuneAnimInstance Interface End...     ***/

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "LocomotionData")
	AKitsuneCharacter* OwningCharacter = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "LocomotionData")
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "LocomotionData")
	float LocomotionDirection;
};
