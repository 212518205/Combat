// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
// [VMREFACTOR] FAbilityUIData 已迁到 UI/Types/AbilityUIData.h（里面不再有 GAS 句柄）
#include "UI/Types/AbilityUIData.h"
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
	UFUNCTION(BlueprintNativeEvent)
	void AbilityEntryInitialize(const FAbilityUIData& Data);

	UFUNCTION(BlueprintNativeEvent)
	void ClearAbilityEntry();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Image_Ability;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Image_TriggerKey;

};
