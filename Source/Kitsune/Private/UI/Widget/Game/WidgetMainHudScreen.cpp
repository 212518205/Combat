// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Game/WidgetMainHudScreen.h"

#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"

void UWidgetMainHudScreen::InitializeMainHudScreen()
{
	CachedPlayerViewModel = UUIManagerSubsystem::GetUIManager(GetOwningPlayer())->GetPlayerViewModel();
	CachedPlayerViewModel->OnHealthPercentChanged.Broadcast();
	CachedPlayerViewModel->OnStaminaPercentChanged.Broadcast();
}
  