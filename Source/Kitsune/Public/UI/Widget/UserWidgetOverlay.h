// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/UserWidgetBase.h"
#include "UserWidgetOverlay.generated.h"

class UUserWidgetHealth;
class UUserWidget;
/**
 * 
 */
UCLASS()
class KITSUNE_API UUserWidgetOverlay : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UUserWidgetOverlay();
	virtual void NativeConstruct() override;

	~UUserWidgetOverlay()
	{
		UE_LOG(LogTemp, Warning, TEXT("UUserWidgetOverlay is Destroyed"));
	}


protected:
};
