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
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Hero_Dodge, "InputTag.Hero.Dodge");

	/*** `@BC`   描述: 角色能力标签   `BC@` ***/
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Sword, "Player.Ability.Equip.Sword");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Sword, "Player.Ability.Unequip.Sword");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Dodge, "Player.Ability.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Weapon_Light, "Player.Ability.Light.Sword");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Weapon_Heavy, "Player.Ability.Heavy.Sword");

	/*** `@BC`   描述: 角色事件标签   `BC@` ***/
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Weapon_Equip_Sword, "Player.Event.Weapon.Equip.Sword");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Weapon_Unequip_Sword, "Player.Event.Weapon.Unequip.Sword");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Dodging, "Player.Status.Dodging");

	/*** `@BC`   描述: 敌人状态标签   `BC@` ***/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Strafing, "Enemy.Status.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Light, "Enemy.Ability.Light");

	/*** `@BC`   描述: Shared 玩家敌人公用   `BC@` ***/
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_DamageBonusMultiplier, "Shared.SetByCaller.DamageBonusMultiplier");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Dead, "Shared.Status.Dead");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact, "Shared.Status.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_UnderAttack, "Shared.Status.UnderAttack");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Dead, "Shared.Ability.Dead");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact, "Shared.Ability.HitReact");

}
