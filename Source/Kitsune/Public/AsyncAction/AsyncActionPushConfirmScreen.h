// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncActionPushConfirmScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmScreenButtonClickedDelegate, EConfirmScreenButtonResult, Result);

/**
 * 
 */
UCLASS()
class KITSUNE_API UAsyncActionPushConfirmScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Function Begin*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"
		, HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push Confirm Screen To Stack"))
	static UAsyncActionPushConfirmScreen* PushConfirmScreen(
		const UObject* WorldContextObject,
		const EConfirmScreenType ScreenType,
		const FText Title, const FText Message
	);

	virtual void Activate() override;

	/** Function End*/

	/** Variable Begin*/
	UPROPERTY(BlueprintAssignable)
	FOnConfirmScreenButtonClickedDelegate OnButtonClicked;
	/** Variable End*/

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	EConfirmScreenType CachedScreenType = EConfirmScreenType::Unknown;
	FText CachedTitle;
	FText CachedMessage;
};
