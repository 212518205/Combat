// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Data/DataAssetStartDataBase.h"
#include "DataAssetStartDataWeapon.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UDataAssetStartDataWeapon : public UDataAssetStartDataBase
{
	GENERATED_BODY()

public:
	/*** [2025年9月23日 16:27:55 来自`@BC@`]
	* TODO: 提供接口 ModifyCharacterData(ACharacter){ 调用修改移动方面数据、属性等 }
	* ResetCharacterData(ACharacter) { CharacterGetDefaultData(包括但不限于移动数据，属性值) }
	***/

	void GiveAbilitiesToASC(UAbilitySystemComponent* TargetASC, int32 AbilityLevel);
	void ClearAbilitiesFromASC(UAbilitySystemComponent* TargetASC) const;

	void GiveWeaponInitialAbilityToASC(UAbilitySystemComponent* TargetASC) const;
	void RemoveWeaponInitialAbilityFromASC(UAbilitySystemComponent* TargetASC) const;

protected:
	/*** `@BC`   描述: 拾取或携带武器后初始能力   `BC@` ***/
	UPROPERTY(EditDefaultsOnly,Category = "Ability", meta = (ForceInlineRow, Categories = "InputTag.Weapon"))
	FAbilityInfo InitialCarriedAbility;
};
