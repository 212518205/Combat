// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/KitsuneInputConfig.h"

#include "FrontendDebugHelper.h"

const UInputAction* UKitsuneInputConfig::FindNativeActionByTag(const FGameplayTag& InTag, const bool bNoFindLog)
{
	for (const auto [InputAction, InputTag] : NativeInputActions)
	{
		if (InputTag == InTag)
		{
			return InputAction;
		}
	}
	if (bNoFindLog)
	{
		Debug::Print(TEXT("InputAction No Find,Tag == ") + InTag.ToString());
	}
	return nullptr;
}

const UInputAction* UKitsuneInputConfig::FindSkillsInputActionByTag(const FGameplayTag& InTag, const bool bNoFindLog)
{
	for (const auto [InputAction, InputTag] : SkillsInputActions)
	{
		if (InputTag == InTag)
		{
			return InputAction;
		}
	}
	if (bNoFindLog)
	{
		Debug::Print(TEXT("InputAction No Find,Tag == ") + InTag.ToString());
	}
	return nullptr;
}
