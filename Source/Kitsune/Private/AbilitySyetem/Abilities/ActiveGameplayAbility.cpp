// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySyetem/Abilities/ActiveGameplayAbility.h"

#include "MotionWarpingComponent.h"

UPlayerCombatComponent* UActiveGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{	
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPlayerCombatComponent>();
}

UMotionWarpingComponent* UActiveGameplayAbility::GetMotionWarpingComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UMotionWarpingComponent>();
}
