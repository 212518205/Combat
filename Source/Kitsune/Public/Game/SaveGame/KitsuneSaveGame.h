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

UCLASS()
class KITSUNE_API UKitsuneSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int32 SaveVersion = 1;
	
	UPROPERTY()
	FPlayerProfileSave PlayerProfile;
	
	UPROPERTY()
	FInventorySave Inventory;
	
	static UKitsuneSaveGame* LoadOrCreate();
	
	void SaveToSlot();
	
};
