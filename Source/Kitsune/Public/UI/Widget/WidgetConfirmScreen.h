// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/WidgetActivatableBase.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "WidgetConfirmScreen.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScreenClosedDelegate, EConfirmScreenButtonResult, Result);

/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetConfirmScreen : public UWidgetActivatableBase
{
	GENERATED_BODY()

public:
	/** Function Begin*/
	void InitScreenInfo(const FText& InTitle, const FText& InMessage,const TArray<FConfirmScreenButtonInfo>& InButtons);
	/** Function End*/

	/** Variable Begin*/
	UPROPERTY(BlueprintAssignable)
	FOnScreenClosedDelegate ScreenClosed;
	/** Variable End*/

private:
	/** Variable Begin*/
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;

	UPROPERTY(meta =  (BindWidget))
	UCommonTextBlock* CommonTextBlock_Message;

	UPROPERTY(meta = (BindWidget))
	UDynamicEntryBox* DynamicEntryBox_Buttons;

	/** Variable End*/
};
