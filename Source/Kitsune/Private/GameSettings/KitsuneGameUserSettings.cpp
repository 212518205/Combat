// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings/KitsuneGameUserSettings.h"

UKitsuneGameUserSettings* UKitsuneGameUserSettings::GetGameUserSettings()
{
	if (GEngine)
	{
		return CastChecked<UKitsuneGameUserSettings>(GEngine->GetGameUserSettings());
	}
	return nullptr;
}

FString UKitsuneGameUserSettings::GetCurrentGameDifficulty()
{
	return CurrentGameDifficulty;
}

void UKitsuneGameUserSettings::SetCurrentGameDifficulty(const FString& InGameDifficulty)
{
	CurrentGameDifficulty = InGameDifficulty;
}
