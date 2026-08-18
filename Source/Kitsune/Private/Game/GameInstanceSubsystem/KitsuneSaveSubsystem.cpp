// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameInstanceSubsystem/KitsuneSaveSubsystem.h"

#include "Game/SaveGame/KitsuneSaveGame.h"
#include "Interfaces/SavableInterface.h"

void UKitsuneSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	CurrentSaveGame = UKitsuneSaveGame::LoadOrCreate();
}

UKitsuneSaveSubsystem* UKitsuneSaveSubsystem::GetSaveSubsystem(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UKitsuneSaveSubsystem>(World->GetGameInstance());
	}
	return nullptr;
}

void UKitsuneSaveSubsystem::RegisterForSaving(const TScriptInterface<ISavableInterface>& Savable)
{
	if (Savable && !SaveTargets.Contains(Savable))
	{
		SaveTargets.Add(Savable);
		Savable->LoadFrom(CurrentSaveGame);
	}
}

void UKitsuneSaveSubsystem::UnRegisterForSaving(const TScriptInterface<ISavableInterface>& Savable)
{
	if (Savable && SaveTargets.Contains(Savable))
	{
		Savable->SaveTo(CurrentSaveGame);
		if (CurrentSaveGame)
		{
			CurrentSaveGame->SaveToSlot();
		}
	}
	SaveTargets.Remove(Savable);
}

void UKitsuneSaveSubsystem::SaveGame()
{
	if (!CurrentSaveGame)
	{
		CurrentSaveGame = UKitsuneSaveGame::LoadOrCreate();
	}
	for (const TScriptInterface<ISavableInterface>& Target : SaveTargets)
	{
		if (Target)
		{
			Target->SaveTo(CurrentSaveGame);
		}
	}
	CurrentSaveGame->SaveToSlot();
}

void UKitsuneSaveSubsystem::LoadGame()
{
	CurrentSaveGame = UKitsuneSaveGame::LoadOrCreate();
	for (const TScriptInterface<ISavableInterface>& Target : SaveTargets)
	{
		if (Target)
		{
			Target->LoadFrom(CurrentSaveGame);
		}
	}
}
