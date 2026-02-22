// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/WidgetActivatableBase.h"
#include "WidgetLocalMenu.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetLocalMenu : public UWidgetActivatableBase
{
	GENERATED_BODY()

protected:
	void NativeOnInitialized() override;
};
