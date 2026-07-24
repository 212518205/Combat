// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTags.h"
#include "WidgetPrimaryLayout.generated.h"

class UKitsuneActivatableWidgetStack;
struct FGameplayTag;
class UCommonActivatableWidget;
class UCommonActivatableWidgetContainerBase;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class KITSUNE_API UWidgetPrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	
public:
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta = (Categories = "UI.WidgetStack")) const FGameplayTag InGameplayTag, UKitsuneActivatableWidgetStack*
	                         InStack);

	UFUNCTION(BlueprintCallable)
	UKitsuneActivatableWidgetStack* FindWidgetStackByTag(const FGameplayTag& InTag)const;
	
	UCommonActivatableWidget* GetTopWidget() const;
	void UpdateInteractState() const;	
private:
	UPROPERTY(Transient)
	TMap<FGameplayTag, UKitsuneActivatableWidgetStack*> GameplayTagToStackMap;
};
