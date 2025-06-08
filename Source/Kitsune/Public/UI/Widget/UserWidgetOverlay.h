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
	FORCEINLINE UUserWidgetHealth* GetHealthWidget() { return WidgetHealth; }
protected:
	UPROPERTY(BlueprintReadWrite,Category="Widget")
	TObjectPtr<UUserWidgetHealth> WidgetHealth;

	UPROPERTY(EditAnywhere,Category="Wdiget|Class")
	TSubclassOf<UUserWidget> WidgetHealthClass;
};
