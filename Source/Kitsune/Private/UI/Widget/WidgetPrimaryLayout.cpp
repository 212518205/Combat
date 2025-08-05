// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WidgetPrimaryLayout.h"

#include "FrontendDebugHelper.h"

void UWidgetPrimaryLayout::RegisterWidgetStack(UPARAM(meta = (Categories = "UI.WidgetStack")) const FGameplayTag InGameplayTag,
	UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime())
	{
		if (!GameplayTagToStackMap.Contains(InGameplayTag))
		{
			GameplayTagToStackMap.Add(InGameplayTag, InStack);
		}
	}
}

UCommonActivatableWidgetContainerBase* UWidgetPrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InTag)const
{
	checkf(GameplayTagToStackMap.Contains(InTag), TEXT("GameplayTagToStackMap no find stack by %s"), *InTag.ToString());
	return GameplayTagToStackMap.FindRef(InTag);
}
