// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/ScriptInterface.h"
#include "Interfaces/SavableInterface.h"
#include "KitsuneSaveSubsystem.generated.h"

class UKitsuneSaveGame;
class ISavableInterface;
/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	static UKitsuneSaveSubsystem* GetSaveSubsystem(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	void RegisterForSaving(const TScriptInterface<ISavableInterface>& Savable);
	
	UFUNCTION(BlueprintCallable)
	void UnRegisterForSaving(const TScriptInterface<ISavableInterface>& Savable);
	
	UFUNCTION(BlueprintCallable)
	void SaveGame();
	
	UFUNCTION(BlueprintCallable)
	void LoadGame();
	
	UFUNCTION(BlueprintPure)
	UKitsuneSaveGame* GetSaveGame() const { return CurrentSaveGame; }
	
private:
	UPROPERTY()
	TObjectPtr<UKitsuneSaveGame> CurrentSaveGame;
	
	UPROPERTY()
	TArray<TScriptInterface<ISavableInterface>> SaveTargets;
	
	
};
