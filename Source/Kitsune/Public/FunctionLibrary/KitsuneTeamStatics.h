// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KitsuneTeamStatics.generated.h"

class UDataAssetFactionMatrix;
/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneTeamStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldObjectContext"))
	static void ApplyFactionPreset(const UObject* WorldObjectContext, UDataAssetFactionMatrix* Matrix);
	
	UFUNCTION(BlueprintPure)
	static int32 GetTeamIDFromActor(const AActor* Actor);
	
	UFUNCTION(BlueprintPure)
	static ETeamAttitude::Type GetAttitude(const AActor* FromActor, const AActor* ToActor);
	
	UFUNCTION(BlueprintPure)
	static bool IsHostile(const AActor* FromActor, const AActor* ToActor);
	
};
