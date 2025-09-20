// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTag/KitsuneGameplayTag.h"


namespace KitsuneGameplayTags
{

	UE_DEFINE_GAMEPLAY_TAG(Message_LiangYiPaste, "Message.LiangYiPaste");
	//Frontend widget stack
	UE_DEFINE_GAMEPLAY_TAG(UI_WidgetStack_ModalStack, "UI.WidgetStack.ModalStack");
	UE_DEFINE_GAMEPLAY_TAG(UI_WidgetStack_GameMenu, "UI.WidgetStack.GameMenu");
	UE_DEFINE_GAMEPLAY_TAG(UI_WidgetStack_HudStack, "UI.WidgetStack.HudStack");
	UE_DEFINE_GAMEPLAY_TAG(UI_WidgetStack_FrontendStack, "UI.WidgetStack.FrontendStack");

	//Frontend widgets
	UE_DEFINE_GAMEPLAY_TAG(UI_Widget_PressAnyKeyScreen, "UI.Widget.PressAnyKeyScreen");
	UE_DEFINE_GAMEPLAY_TAG(UI_Widget_MainMenuScreen, "UI.Widget.MainMenuScreen");
	UE_DEFINE_GAMEPLAY_TAG(UI_Widget_OptionsScreen, "UI.Widget.OptionsScreen");
	UE_DEFINE_GAMEPLAY_TAG(UI_Widget_ConfirmScreen, "UI.Widget.ConfirmScreen");
	UE_DEFINE_GAMEPLAY_TAG(UI_Widget_MainHudScreen, "UI.Widget.MainHudScreen");

	/** Input Tag*/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_Light, "InputTag.Attack.Light");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Heavy, "InputTag.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump");

}
