// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "KitsuneFactionSubsystem.generated.h"

class UDataAssetFactionMatrix;
/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneFactionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	static UKitsuneFactionSubsystem* GetFactionSubsystem(const UObject* WorldObjectContext);
	
	UFUNCTION(BlueprintCallable)
	void ApplyPreset(UDataAssetFactionMatrix* Matrix);
	
	UFUNCTION(BlueprintCallable)
	void SetAttitude(const uint8 FromTeamID, const uint8 ToTeamID, const ETeamAttitude::Type Attitude, const bool bBidirectional);
	
	UFUNCTION(BlueprintPure)
	ETeamAttitude::Type GetAttitude(const uint8 FromTeamID, const uint8 ToTeamID);
	
protected:
	UFUNCTION(BlueprintPure)
	bool IsServer() const;
	
	TMap<uint8, FFactionRelation> RuntimeTable;
	
};
