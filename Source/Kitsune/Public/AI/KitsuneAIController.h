// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "KitsuneAIController.generated.h"

class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class KITSUNE_API AKitsuneAIController : public AAIController
{
	GENERATED_BODY()

public:
	explicit AKitsuneAIController(const FObjectInitializer& ObjectInitializer);

	/***   ...IGenericTeamAgentInterface Interface Begin...   ***/
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	/***   ...IGenericTeamAgentInterface Interface End...     ***/

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAISenseConfig_Sight> SenseConfig_Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> EnemyPerceptionComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config")
	bool bEnableDetourCrowdAvoidance = true;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMax = "4", UIMin = "1"))
	int32 DetourCrowdAvoidanceQuality = 1;
};
