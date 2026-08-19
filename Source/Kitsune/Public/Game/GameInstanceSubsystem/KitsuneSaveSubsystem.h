// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/ScriptInterface.h"
#include "Interfaces/SavableInterface.h"
#include "KitsuneSaveSubsystem.generated.h"

class UGlobalSaveGame;
class UKitsuneSaveGame;
class ISavableInterface;

USTRUCT()
struct FSaveGameContext
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<TScriptInterface<ISavableInterface>> Targets;
	
	UPROPERTY()
	TObjectPtr<UKitsuneSaveGame> SaveGame;
};

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	static UKitsuneSaveSubsystem* GetSaveSubsystem(const UObject* WorldContextObject);
	static int64 ResolvePlayerCredential(const FString& InCredential);
	
	UFUNCTION(BlueprintCallable)
	void RegisterForSaving(const int64 PlayerUID, const TScriptInterface<ISavableInterface>& Savable);
	
	UFUNCTION(BlueprintCallable)
	void UnRegisterForSaving(const TScriptInterface<ISavableInterface>& Savable);
	
	UFUNCTION(BlueprintCallable)
	void SaveGameForPlayer(int64 PlayerUID);
	
	UFUNCTION(BlueprintCallable)
	void LoadGameForPlayer(int64 PlayerUID);
	
	void MarkDirty(const int64 PlayerUID);
	
	UFUNCTION(BlueprintPure)
	UGlobalSaveGame* GetGlobalSaveGame() const { return CurrentSaveGame; }
	FString GetOrCreateLocalCredential();
	UKitsuneSaveGame* GetOrCreatePlayerSaveGame(const int64 PlayerUID);
	
private:
	void FlushDirtySaves();
	
	UPROPERTY()
	TObjectPtr<UGlobalSaveGame> CurrentSaveGame;
	
	UPROPERTY()
	TMap<int64, FSaveGameContext> PlayerSaveGameContexts;
	
	UPROPERTY()
	TSet<int64> DirtyUIDs;
	
	UPROPERTY()
	FString Credential;
	
	FTimerHandle SaveTimerHandle;
	
	float SaveIntervalSeconds = 3.f;	
	
};
