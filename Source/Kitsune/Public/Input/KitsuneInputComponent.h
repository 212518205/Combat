// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "KitsuneInputConfig.h"
#include "KitsuneInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject, typename CallBackFunc>
	void BindAbilityInputAction(UKitsuneInputConfig* InputConfig, UserObject* ObjectContext,
	                            CallBackFunc CallBackPressed, CallBackFunc CallBackReleased);

};

template <class UserObject, typename CallBackFunc>
void UKitsuneInputComponent::BindAbilityInputAction(UKitsuneInputConfig* InputConfig, UserObject* ObjectContext,
	CallBackFunc CallBackPressed, CallBackFunc CallBackReleased)
{
	check(InputConfig);

	for (auto& [InputAction, InputTag]:InputConfig->InputActions)
	{
		if (CallBackPressed)
		{
			BindAction(InputAction, ETriggerEvent::Started, ObjectContext, CallBackPressed, InputTag);
		}
		if (CallBackReleased)
		{
			BindAction(InputAction, ETriggerEvent::Completed, ObjectContext, CallBackReleased, InputTag);
		}
	}
}

