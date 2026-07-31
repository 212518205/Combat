// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTags.h"
#include "AsyncActionPushSoftWidget.generated.h"

struct FGameplayTag;
class UWidgetActivatableBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, UWidgetActivatableBase*, PushedWidget);

/**
 * 
 */
UCLASS()
class KITSUNE_API UAsyncActionPushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Function Begin-->*/
	UFUNCTION(BlueprintCallable,meta=(WorldContext="WorldContextObject"
		,HidePin="WorldContextObject",BlueprintInternalUseOnly="true",DisplayName="Push Soft Widget To Widget Stack"))
	static UAsyncActionPushSoftWidget* PushSoftWidgetByTag(const UObject* WorldContextObject,
	                                                  APlayerController* OwningPlayerController,
	                                                  UPARAM(meta = (Categories = "UI.Widget")) FGameplayTag InWidgetTag,
	                                                  UPARAM(meta = (Categories = "UI.WidgetStack")) FGameplayTag InWidgetStackTag,
	                                                  bool bFocusOnNewlyPushedWidget = true
	);

	virtual void Activate() override;
	
	
	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate AfterPush;

private:
	/** Variable Begin*/ 
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	TWeakObjectPtr<APlayerController> CachedOwningPC;
	FGameplayTag CachedWidgetTag;
	FGameplayTag CachedWidgetStackTag;
	bool bCachedFocusOnNewlyPushedWidget = false;
	/** Variable End*/
};
