// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/AnimInstance/KitsuneAnimInstance.h"
#include "LinkedLayerAnimInstance.generated.h"

class UHeroAnimInstance;
/**
 * 
 */
UCLASS()
class KITSUNE_API ULinkedLayerAnimInstance : public UKitsuneAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UHeroAnimInstance* GetHeroAnimInstance() const;
};
