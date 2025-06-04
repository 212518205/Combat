// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KitsuneWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneWidgetController : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(EditAnywhere)
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAttributeSet> AttributeSet;

};
