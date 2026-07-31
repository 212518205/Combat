// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WidgetPrimaryLayout.h"

#include "CommonActivatableWidget.h"
#include "FunctionLibrary/KitsuneFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "UI/KitsuneActivatableWidgetStack.h"
#include "UI/Widget/Game/WidgetMainHudScreen.h"
#include "Widgets/CommonActivatableWidgetContainer.h"


void UWidgetPrimaryLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UWidgetPrimaryLayout::RegisterWidgetStack(UPARAM(meta = (Categories = "UI.WidgetStack")) const FGameplayTag InGameplayTag,
                                               UKitsuneActivatableWidgetStack* InStack)
{
	if (IsDesignTime())return;
	
	if (!GameplayTagToStackMap.Contains(InGameplayTag))
	{
		GameplayTagToStackMap.Add(InGameplayTag, InStack);
	}
	
}

UKitsuneActivatableWidgetStack* UWidgetPrimaryLayout::FindWidgetStackByTag(UPARAM(meta = (Categories = "UI.WidgetStack")) const FGameplayTag InTag)const
{
	checkf(GameplayTagToStackMap.Contains(InTag), TEXT("GameplayTagToStackMap no find stack by %s"), *InTag.ToString());
	return GameplayTagToStackMap.FindRef(InTag);
}

void UWidgetPrimaryLayout::ClearWidgetStackByTag(UPARAM(meta = (Categories = "UI.WidgetStack")) const FGameplayTag InTag) const
{
	UKitsuneActivatableWidgetStack* WidgetStack = FindWidgetStackByTag(InTag);
	if (!WidgetStack)return;
	
	WidgetStack->ClearStack();
}

void UWidgetPrimaryLayout::PopWidgetStackByTag(const FGameplayTag InTag) const
{
	UKitsuneActivatableWidgetStack* WidgetStack = FindWidgetStackByTag(InTag);;
	if (!WidgetStack)return;
	
	WidgetStack->PopWidget();
}

UCommonActivatableWidget* UWidgetPrimaryLayout::GetTopWidget() const
{
	FGameplayTag StackTags[3] = {
		KitsuneGameplayTags::UI_WidgetStack_HudStack,
		KitsuneGameplayTags::UI_WidgetStack_GameMenu,
		KitsuneGameplayTags::UI_WidgetStack_ModalStack
	};
	UCommonActivatableWidget* TopWidget = nullptr;
	
	for (FGameplayTag& StackTag	: StackTags)
	{
		if (UCommonActivatableWidget* DisplayWidget = GameplayTagToStackMap.FindRef(StackTag)->GetActiveWidget(); DisplayWidget && DisplayWidget->IsActivated())
		{
			TopWidget = DisplayWidget;
		}
	}
	
	return TopWidget;
}

void UWidgetPrimaryLayout::UpdateInteractState() const
{
	if (const UCommonActivatableWidget* TopWidget = GetTopWidget(); TopWidget && TopWidget->IsA(UWidgetMainHudScreen::StaticClass()))
	{
		UKitsuneFunctionLibrary::AddGameplayTagToActorIfNone(GetOwningPlayerPawn(), KitsuneGameplayTags::Player_Status_Pickupable);
	}else
	{
		UKitsuneFunctionLibrary::TryRemoveGameplayTagFromActor(GetOwningPlayerPawn(), KitsuneGameplayTags::Player_Status_Pickupable);
	}
	
}
