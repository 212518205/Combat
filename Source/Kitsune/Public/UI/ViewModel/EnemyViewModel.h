// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/AttributeViewModel.h"
#include "EnemyViewModel.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UEnemyViewModel : public UAttributeViewModel
{
	GENERATED_BODY()

public:
	virtual void NativeInitialize() override;
};
