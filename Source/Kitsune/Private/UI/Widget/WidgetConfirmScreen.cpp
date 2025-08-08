// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WidgetConfirmScreen.h"
#include "UI/Widget/Button/KitsuneCommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;

	FConfirmScreenButtonInfo OKButtonInfo;
	OKButtonInfo.ButtonResult = EConfirmScreenButtonResult::Confirmed;
	OKButtonInfo.ButtonTextDisplay = FText::FromString(TEXT("是"));

	InfoObject->AvailableScreenButtons.Add(OKButtonInfo);

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;

	FConfirmScreenButtonInfo YesButtonInfo;
	YesButtonInfo.ButtonResult = EConfirmScreenButtonResult::Confirmed;
	YesButtonInfo.ButtonTextDisplay = FText::FromString(TEXT("确认"));

	FConfirmScreenButtonInfo NoButtonInfo;
	NoButtonInfo.ButtonResult = EConfirmScreenButtonResult::Cancelled;
	NoButtonInfo.ButtonTextDisplay = FText::FromString(TEXT("取消"));

	InfoObject->AvailableScreenButtons.Add(YesButtonInfo);
	InfoObject->AvailableScreenButtons.Add(NoButtonInfo);

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;

	FConfirmScreenButtonInfo OkButtonInfo;
	OkButtonInfo.ButtonResult = EConfirmScreenButtonResult::Confirmed;
	OkButtonInfo.ButtonTextDisplay = FText::FromString(TEXT("是"));

	FConfirmScreenButtonInfo CancelButtonInfo;
	CancelButtonInfo.ButtonResult = EConfirmScreenButtonResult::Cancelled;
	CancelButtonInfo.ButtonTextDisplay = FText::FromString(TEXT("否"));

	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	InfoObject->AvailableScreenButtons.Add(CancelButtonInfo);

	return InfoObject;
}

void UWidgetConfirmScreen::InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject, TFunction<void(EConfirmScreenButtonResult)> ClickedButtonCallback)
{
	check(InScreenInfoObject && CommonTextBlock_Title && CommonTextBlock_Message && DynamicEntryBox_Buttons);

	CommonTextBlock_Title->SetText(InScreenInfoObject->ScreenTitle);
	CommonTextBlock_Message->SetText(InScreenInfoObject->ScreenMessage);

	//Checking if the entry box has old button created previously
	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		DynamicEntryBox_Buttons->Reset<UKitsuneCommonButtonBase>(
			[](UKitsuneCommonButtonBase& ExistingButton)
			{
				ExistingButton.OnClicked().Clear();
			}
		);
	}

	check(!InScreenInfoObject->AvailableScreenButtons.IsEmpty());

	for (const FConfirmScreenButtonInfo& AvailableButtonInfo : InScreenInfoObject->AvailableScreenButtons)
	{
		const UKitsuneCommonButtonBase* AddedButton = DynamicEntryBox_Buttons->CreateEntry<UKitsuneCommonButtonBase>();
		AddedButton->SetButtonText(AvailableButtonInfo.ButtonTextDisplay);
		AddedButton->OnClicked().AddLambda(
			[ClickedButtonCallback, AvailableButtonInfo, this]()
			{
				ClickedButtonCallback(AvailableButtonInfo.ButtonResult);

				DeactivateWidget();
			}
		);
	}
}