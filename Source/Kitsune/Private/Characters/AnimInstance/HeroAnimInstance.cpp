// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AnimInstance/HeroAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Characters/KitsuneCharacter.h"

void UHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<ACharacterBase>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	GroundSpeed = static_cast<float>(OwningCharacter->GetVelocity().Size2D());

	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());
}
