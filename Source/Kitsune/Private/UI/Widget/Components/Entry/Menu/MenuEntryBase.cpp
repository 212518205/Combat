// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/Entry/Menu/MenuEntryBase.h"

#include "CommonTextBlock.h"
#include "UIManagerSubsystem.h"
#include "Components/Image.h"
#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "UI/DataObjects/Inventory/MenuEntryData.h"
#include "UI/Widget/WidgetActivatableBase.h"

void UMenuEntryBase::NativeOnEntryInitialize(UObject* ListItemObject)
{
	if (UMenuEntryData* EntryData = Cast<UMenuEntryData>(ListItemObject))
	{
		CachedEntryData = EntryData;

		Image_DisplayImage->SetBrushFromTexture(EntryData->DisplayIcon);
		CommonTextBlock_DisplayName->SetText(EntryData->DisplayName);
		CommonTextBlock_HotKeyText->SetText(EntryData->HotKeyText);
		CommonTextBlock_ShowAsNewText->SetText(EntryData->ShowAsNewText);
	}

}

void UMenuEntryBase::OnHovered()
{
	Image_DisplayImage->SetColorAndOpacity(HoverIconColor);

	Super::OnHovered();
}

void UMenuEntryBase::OnUnHovered()
{
	Image_DisplayImage->SetColorAndOpacity(DefaultIconColor);

	Super::OnUnHovered();
}

void UMenuEntryBase::OnPressed()
{
	Image_DisplayImage->SetColorAndOpacity(ClickIconColor);

	Super::OnPressed();
}

void UMenuEntryBase::OnReleased()
{
	Image_DisplayImage->SetColorAndOpacity(DefaultIconColor);
	const UUIManagerSubsystem* UIManager = UUIManagerSubsystem::GetUIManager(GetWorld());
	UIManager->PushSoftWidgetToStackAsync(
		KitsuneGameplayTags::UI_WidgetStack_GameMenu,
		UFrontendBlueprintFunctionLibrary::GetScreenSoftWidgetClassByTag(CachedEntryData->ScreenToOpen),
		[this](EAsyncPushWidgetState InPushState, UWidgetActivatableBase* PushedWidget)
		{
			switch (InPushState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				PushedWidget->SetOwningPlayer(GetOwningPlayer());
				break;

			case EAsyncPushWidgetState::AfterPush:
				break;

			default:
				break;
			}
		});
	
	Super::OnReleased();
}
