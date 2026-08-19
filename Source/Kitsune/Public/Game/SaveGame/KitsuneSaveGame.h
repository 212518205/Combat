// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "GameFramework/SaveGame.h"
#include "KitsuneSaveGame.generated.h"

USTRUCT()
struct FPlayerProfileSave
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int32 PlayerLevel = 1;
	
	UPROPERTY()
	FString PlayerName = TEXT("北初");
	
};

USTRUCT()
struct FInventorySave
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FSerializedItem> Items;
	
	UPROPERTY()
	TArray<FCategoryCapacityEntry> InventoryCapacity;
	
};

USTRUCT()
struct FGlobalSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	int64 NextPlayerUID = 1000;
	
	UPROPERTY()
	TArray<int64> KnownPlayerUIDs;
	
	UPROPERTY()
	TMap<FString, int64> PlayerCredentialToUID;
	
};

UCLASS()
class KITSUNE_API UKitsuneSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int32 SaveVersion = 2;
	
	UPROPERTY()
	FPlayerProfileSave PlayerProfile;
	
	UPROPERTY()
	FInventorySave Inventory;
	
	static UKitsuneSaveGame* LoadOrCreate(const FString& SlotName);	
	static const FString& GetGlobalSlotName();
	static FString MakePlayerSlotName(const int64 PlayerUID);
	void SaveToSlot(const FString& SlotName);
	
};

UCLASS()
class UGlobalSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGlobalSaveData Data;
	
	static UGlobalSaveGame* LoadOrCreate();
	
	void SaveToSlot();
	
};
