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
	}
}

void UKitsuneSaveSubsystem::UnRegisterForSaving(const TScriptInterface<ISavableInterface>& Savable)
{
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
		// 【修正】判空，避免失效接口(对象已销毁但未注销)解引用崩溃
		if (Target)
		{
			Target->SaveTo(CurrentSaveGame);
		}
	}
	// 【修正】真正落盘：把内存里的数据写进 slot，否则存档不生效
	CurrentSaveGame->SaveToSlot();
}

void UKitsuneSaveSubsystem::LoadGame()
{
	CurrentSaveGame = UKitsuneSaveGame::LoadOrCreate();
	for (const TScriptInterface<ISavableInterface>& Target : SaveTargets)
	{
		// 【修正】判空
		if (Target)
		{
			Target->LoadFrom(CurrentSaveGame);
		}
	}
}
