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
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_Light, "InputTag.Weapon.Light");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_Heavy, "InputTag.Weapon.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_Unequip, "InputTag.Weapon.Unequip");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Hero_Jump, "InputTag.Hero.Jump");

	/*** `@BC`   描述: 角色能力标签   `BC@` ***/
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Sword_Equip, "Player.Ability.Sword.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Sword_Unequip, "Player.Ability.Sword.Unequip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Sword_Attack_Light, "Player.Ability.Sword.Attack.Light");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Sword_Attack_Heavy, "Player.Ability.Sword.Attack.Heavy");

	/*** `@BC`   描述: 角色事件标签   `BC@` ***/
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Sword_Equip, "Player.Event.Sword.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Sword_Unequip, "Player.Event.Sword.Unequip");
}
