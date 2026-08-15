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

void UMenuEntryBase::NativeOnHovered()
{
	Super::NativeOnHovered();
	Image_DisplayImage->SetColorAndOpacity(HoverIconColor);
}

void UMenuEntryBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	Image_DisplayImage->SetColorAndOpacity(DefaultIconColor);
}

void UMenuEntryBase::NativeOnClicked()
{
	Super::NativeOnClicked();
	Image_DisplayImage->SetColorAndOpacity(DefaultIconColor);
	// ===== [修改] 原 OnReleased 的跳转逻辑移入 NativeOnClicked（CommonButtonBase 的点击完成回调） =====
	const UUIManagerSubsystem* UIManager = UUIManagerSubsystem::GetUIManager(GetWorld());
	UIManager->PushSoftWidgetToStackAsync(
		KitsuneGameplayTags::UI_WidgetStack_GameMenu,
		CachedEntryData->ScreenToOpen,
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
}
