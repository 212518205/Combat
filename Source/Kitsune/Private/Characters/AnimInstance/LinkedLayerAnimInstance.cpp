// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AnimInstance/LinkedLayerAnimInstance.h"

#include "Characters/AnimInstance/HeroAnimInstance.h"

UHeroAnimInstance* ULinkedLayerAnimInstance::GetHeroAnimInstance() const
{
	return CastChecked<UHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
