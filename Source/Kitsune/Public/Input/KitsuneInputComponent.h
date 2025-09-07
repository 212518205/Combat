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
	template<class UserClass,typename PressedFuncType, typename ReleaseFuncType, typename HeldFuncType>
	void BindAbilityActions(const UKitsuneInputConfig* InputConfig, UserClass* Object,
		PressedFuncType PressedFunc, ReleaseFuncType ReleaseFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleaseFuncType, typename HeldFuncType>
void UKitsuneInputComponent::BindAbilityActions(const UKitsuneInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleaseFuncType ReleaseFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const auto [InputAction, InputTag]:InputConfig->InputActions)
	{
		if (InputAction&& InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(InputAction, ETriggerEvent::Started, Object, PressedFunc, InputTag);
			}
			if (ReleaseFunc)
			{
				BindAction(InputAction, ETriggerEvent::Completed, Object, ReleaseFunc, InputTag);
			}
			if (HeldFunc)
			{
				BindAction(InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputTag);
			}
		}
	}
}
