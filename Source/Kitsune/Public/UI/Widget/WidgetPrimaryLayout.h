// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTags.h"
#include "WidgetPrimaryLayout.generated.h"

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
public:
	virtual void NativeOnInitialized() override;
	
	/** Function Begin*/
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta = (Categories = "UI.WidgetStack")) const FGameplayTag InGameplayTag, UCommonActivatableWidgetContainerBase* InStack);

	UFUNCTION(BlueprintCallable)
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag& InTag)const;

	UFUNCTION(BlueprintCallable)
	void DeActivableWidgetStackByTag(const FGameplayTag& InTag) const;
	
	UCommonActivatableWidget* GetTopWidget() const;
	void UpdateInteractState() const;	
	/** Function End*/
private:
	/** Variable Begin*/
	UPROPERTY(Transient)
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> GameplayTagToStackMap;
	/** Variable End*/
};
