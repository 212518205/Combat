// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AnimInstance/KitsuneAnimInstance.h"

#include "FunctionLibrary/KitsuneFunctionLibrary.h"

bool UKitsuneAnimInstance::DoesHaveGameplayTag(const FGameplayTag CheckTag) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UKitsuneFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, CheckTag);
	}

	return false;
}
