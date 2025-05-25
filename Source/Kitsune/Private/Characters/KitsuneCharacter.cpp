// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/KitsuneCharacter.h"
#include"GameFramework/CharacterMovementComponent.h"

AKitsuneCharacter::AKitsuneCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}
