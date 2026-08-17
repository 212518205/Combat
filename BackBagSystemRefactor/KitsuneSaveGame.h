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
	int32 PlayerLevel = 1;   // 【修正】给默认值，避免新建存档对象时读未初始化内存

	UPROPERTY()
	FString PlayerName;

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
