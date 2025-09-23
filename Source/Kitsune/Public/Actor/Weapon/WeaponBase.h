// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Actor/ItemBase.h"
#include "WeaponBase.generated.h"


class UDataAssetStartDataWeapon;
class UKitsuneAbilitySystemComponent;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class KITSUNE_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

public:
	AWeaponBase();

	/*** REFACTOR: 需修改接口名字
	 *	EquipWeaponToCharacter{调用GiveWeaponAbilitiesToASC && ModifyCharacterData }
	 *	UnequipWeaponFromCharacter{ 调用ClearWeaponAbilityFromASC }
	 *	将下面两个函数替换
	 *. [2025年9月23日 16:12:01 来自`@BC@`] ***/

	void GiveWeaponAbilitiesToASC(UAbilitySystemComponent* TargetASC) const;
	void ClearWeaponAbilitiesFromASC(UAbilitySystemComponent* TargetASC) const;

	/*** TODO: 须保留给生成或拾取时由CombatComponent中RegisterWeapon调用. [2025年9月23日 16:16:31 来自`@BC@`] ***/
	void GiveWeaponInitialAbilityToASC(UAbilitySystemComponent* TargetASC) const;

protected:
	/*** `@BC`   描述: 	基类基础上添加盒体检测   `BC@` ***/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UBoxComponent> ItemOverlapBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Ability System")
	TObjectPtr<UDataAssetStartDataWeapon> WeaponDataInfo;

	int32 WeaponLevel = 1;
};
