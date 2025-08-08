// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WidgetConfirmScreen.h"
#include "UI/Widget/Button/KitsuneCommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"

void UWidgetConfirmScreen::InitScreenInfo(const FText& InTitle, const FText& InMessage,const TArray<FConfirmScreenButtonInfo>& InButtons)
{
	CommonTextBlock_Title->SetText(InTitle);
	CommonTextBlock_Message->SetText(InMessage);
	DynamicEntryBox_Buttons->Reset();
	for (const FConfirmScreenButtonInfo& ButtonInfo:InButtons)
	{
		if (const UKitsuneCommonButtonBase* Button = DynamicEntryBox_Buttons->CreateEntry<UKitsuneCommonButtonBase>())
		{
			Button->SetButtonText(ButtonInfo.ButtonTextDisplay);
			Button->OnClicked().AddLambda(
				[this,ButtonInfo]()
				{
					ScreenClosed.Broadcast(ButtonInfo.ButtonResult);
					DeactivateWidget();
				});
		}
	}
}
