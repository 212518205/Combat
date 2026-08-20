// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "WidgetAbilityList.generated.h"

class UWidgetAbilityEntry;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetAbilityList : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void FillAbilityList(const TArray<FAbilityUIData>& InAbilityUIDates);
	
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "FillAbilityList"))
	void BP_FillAbilityList();
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Up;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Down;
	
	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TSubclassOf<UWidgetAbilityEntry> AbilityEntryClass;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FAbilityUIData> AbilityUIDates;
	
};
