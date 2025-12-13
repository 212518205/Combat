// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_OrientToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UBTService_OrientToTargetActor : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_OrientToTargetActor();

	/***   ...BTNode Interface Begin...   ***/
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	/***   ...BTNode Interface End...     ***/

	/***   ...BTService Interface Begin...   ***/
	virtual FString GetStaticDescription() const override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	/***   ...BTService Interface End...     ***/


protected:
	UPROPERTY(EditAnywhere, Category = "Target")
	float RotationInterpSpeed = 0.f;

	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector InTargetActorKey;

};
