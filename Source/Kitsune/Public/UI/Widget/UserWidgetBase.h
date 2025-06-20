// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetBase.generated.h"

class UCharacterStateViewModel;
/**
 * 
 */
UCLASS()
class KITSUNE_API UUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly,Category="ViewModel")
	TObjectPtr<UCharacterStateViewModel> CharStateVM;
};
