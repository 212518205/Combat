// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Button/KitsuneCommonButtonBase.h"

#include "UIManagerSubsystem.h"

void UKitsuneCommonButtonBase::SetButtonText(const FText InText) const
{
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(InText);
	}
}

void UKitsuneCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);
}

void UKitsuneCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (CommonTextBlock_ButtonText && GetCurrentTextStyleClass())
	{
		CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
}

void UKitsuneCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (const UUIManagerSubsystem* UIManagerSubsystem=UUIManagerSubsystem::GetUIManager(this))
	{
		UIManagerSubsystem->ButtonDescriptionUpdateDelegate.Broadcast(this, ButtonDescriptionText);
	}
}

void UKitsuneCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	if (const UUIManagerSubsystem* UIManagerSubsystem = UUIManagerSubsystem::GetUIManager(this))
	{
		UIManagerSubsystem->ButtonDescriptionUpdateDelegate.Broadcast(this, FText::GetEmpty());
	}
}
