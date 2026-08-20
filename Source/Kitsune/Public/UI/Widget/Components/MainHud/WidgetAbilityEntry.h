// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "WidgetAbilityEntry.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetAbilityEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void AbilityInitialize(const FAbilityUIData& Data);
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Image_Ability;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Key;
	
};
