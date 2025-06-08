// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "KitsuneHUD.generated.h"

class UUserWidgetOverlay;
class UAttributeSet;
class UAbilitySystemComponent;
class UUserWidgetBase;
/**
 * 
 */
UCLASS()
class KITSUNE_API AKitsuneHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitCharacterState(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UUserWidgetOverlay> CharStateWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidgetOverlay> OverlayWidgetClass;
};
