// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTags.h"
#include "WidgetPrimaryLaout.generated.h"

struct FGameplayTag;
class UCommonActivatableWidgetContainerBase;
/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetPrimaryLaout : public UCommonUserWidget
{
	GENERATED_BODY()
protected:
	/** Function Begin*/
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta = (Categories = "UI.WidgetStack")) const FGameplayTag InGameplayTag, UCommonActivatableWidgetContainerBase* InStack);

	UFUNCTION(BlueprintCallable)
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag& InTag)const;
	/** Function End*/
private:
	/** Variable Begin*/
	UPROPERTY(Transient)
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> GameplayTagToStackMap;
	/** Variable End*/
};
