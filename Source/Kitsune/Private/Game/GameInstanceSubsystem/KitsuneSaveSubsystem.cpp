// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameInstanceSubsystem/KitsuneSaveSubsystem.h"

#include "Game/GameInstance/KitsuneGameInstance.h"
#include "Game/SaveGame/KitsuneSaveGame.h"
#include "Interfaces/SavableInterface.h"

void UKitsuneSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	CurrentSaveGame = UGlobalSaveGame::LoadOrCreate();
}

void UKitsuneSaveSubsystem::Deinitialize()
{
	FlushDirtySaves();
	
	Super::Deinitialize();
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

void UKitsuneSaveSubsystem::RegisterForSaving(const int64 PlayerUID, const TScriptInterface<ISavableInterface>& Savable)
{
	if (PlayerUID <= 0 || !Savable) return;
	
	auto& [Targets, SaveGame] = PlayerSaveGameContexts.FindOrAdd(PlayerUID);
	if (!SaveGame)
	{
		SaveGame = UKitsuneSaveGame::LoadOrCreate(UKitsuneSaveGame::MakePlayerSlotName(PlayerUID));
	}
	if (!Targets.Contains(Savable))
	{
		Targets.Add(Savable);
	}
}

void UKitsuneSaveSubsystem::UnRegisterForSaving(const TScriptInterface<ISavableInterface>& Savable)
{
	if (!Savable)return;
	const int64 PlayerUID = Savable->GetSavePlayerUID();
	if (PlayerUID <= 0)return;
	if (FSaveGameContext* Found = PlayerSaveGameContexts.Find(PlayerUID))
	{
		if (Found->Targets.Remove(Savable) > 0 && Found->SaveGame)
		{
			Savable->SaveTo(Found->SaveGame);
			Found->SaveGame->SaveToSlot(UKitsuneSaveGame::MakePlayerSlotName(PlayerUID));
		}
	}
}

void UKitsuneSaveSubsystem::SaveGameForPlayer(const int64 PlayerUID)
{
	if (FSaveGameContext* Found = PlayerSaveGameContexts.Find(PlayerUID))
	{
		for (const TScriptInterface<ISavableInterface>& Target : Found->Targets)
		{
			if (Target)
			{
				Target->SaveTo(Found->SaveGame);
			}
		}
		Found->SaveGame->SaveToSlot(UKitsuneSaveGame::MakePlayerSlotName(PlayerUID));
	}
}

void UKitsuneSaveSubsystem::LoadGameForPlayer(const int64 PlayerUID)
{
	if (FSaveGameContext* Found = PlayerSaveGameContexts.Find(PlayerUID))
	{
		for (const TScriptInterface<ISavableInterface>& Target : Found->Targets)
		{
			if (Target)
			{
				Target->LoadFrom(Found->SaveGame);
			}
		}
	}
}

void UKitsuneSaveSubsystem::MarkDirty(const int64 PlayerUID)
{
	if (PlayerUID <= 0)return;
	DirtyUIDs.Add(PlayerUID);
	
	if (const UWorld* World = GetGameInstance()->GetWorld())
	{
		if (!World->GetTimerManager().IsTimerActive(SaveTimerHandle))
		{
			World->GetTimerManager().SetTimer(SaveTimerHandle, this, &ThisClass::FlushDirtySaves, SaveIntervalSeconds, false);
		}
	}
}

FString UKitsuneSaveSubsystem::GetOrCreateLocalCredential()
{
	if (const UKitsuneGameInstance* KitsuneGI = Cast<UKitsuneGameInstance>(GetGameInstance()))
	{
		if (!KitsuneGI->GetPIEOverrideCredential().IsEmpty())
		{
			return KitsuneGI->GetPIEOverrideCredential();
		}
	}
	
#if WITH_EDITOR
	if (GIsEditor)
	{
		if (const UWorld* World = GetGameInstance()->GetWorld())
		{
			if (World->GetNetMode() == NM_Client)
			{
				return FGuid::NewGuid().ToString();
			}
		}
	}
#endif
	
	const FString Path = FPaths::ProjectSavedDir() / TEXT("LocalCredential.txt");
	if (FString Existing; FFileHelper::LoadFileToString(Existing, *Path) && !Existing.IsEmpty())
	{
		return Existing;	
	}
	
	FString NewCredential = FGuid::NewGuid().ToString();
	FFileHelper::SaveStringToFile(NewCredential, *Path);
	return NewCredential;
	
}

int64 UKitsuneSaveSubsystem::ResolvePlayerCredential(const FString& InCredential)
{
	UGlobalSaveGame* GS = UGlobalSaveGame::LoadOrCreate();
	if (!GS || InCredential.IsEmpty())return -1;
	
	auto& [NextPlayerUID, KnownPlayerUIDs, PlayerCredentialToUID] = GS->Data;
	if (const int64* Found = PlayerCredentialToUID.Find(InCredential))
	{
		return *Found;
	}

	const int64 NewPlayerUID = ++NextPlayerUID;
	KnownPlayerUIDs.Add(NewPlayerUID);
	PlayerCredentialToUID.Add(InCredential, NewPlayerUID);
	
	GS->SaveToSlot();
	
	return NewPlayerUID;
}

UKitsuneSaveGame* UKitsuneSaveSubsystem::GetOrCreatePlayerSaveGame(const int64 PlayerUID)
{
	if (FSaveGameContext* Found = PlayerSaveGameContexts.Find(PlayerUID))
	{
		if (Found->SaveGame)
		{
			return Found->SaveGame;
		}
	}
	auto& [Targets, SaveGame] = PlayerSaveGameContexts.FindOrAdd(PlayerUID);
	if (!SaveGame)
	{
		SaveGame = UKitsuneSaveGame::LoadOrCreate(UKitsuneSaveGame::MakePlayerSlotName(PlayerUID));
	}
	return SaveGame;
}

void UKitsuneSaveSubsystem::FlushDirtySaves()
{
	for (const int64 PlayerUID : DirtyUIDs)
	{
		SaveGameForPlayer(PlayerUID);
	}
	
	DirtyUIDs.Reset();
}













