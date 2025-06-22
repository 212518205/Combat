// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"

class UGameplayEffect;
class USphereComponent;

UCLASS()
class KITSUNE_API AEffectActor : public AActor
{
	GENERATED_BODY()
	
public:
	/*Public Function Begin*/
	AEffectActor();
	/*Public Function End*/

protected:
	/*Protected Function Begin*/
	UFUNCTION(BlueprintCallable,Category="Effect|Apply")
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GEClass);
	/*Protected Function End*/

	/*Protected Variable Begin*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Effect|Class")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	/*Protected Variable End*/
};
