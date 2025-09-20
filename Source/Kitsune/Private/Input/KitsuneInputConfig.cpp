// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/KitsuneInputConfig.h"

#include "FrontendDebugHelper.h"

const UInputAction* UKitsuneInputConfig::FindInputActionByTag(const FGameplayTag& InTag, const bool bNoFindLog)
{
	for (const auto [InputAction, InputTag]:InputActions)
	{
		if (InputAction&&InputTag.IsValid())
		{
			if (InputTag==InTag)
			{
				return InputAction;
			}
		}
	}
	if (bNoFindLog)
	{
		Debug::Print(TEXT("InputAction No Find,Tag == ") + InTag.ToString());
	}
	return nullptr;
}
