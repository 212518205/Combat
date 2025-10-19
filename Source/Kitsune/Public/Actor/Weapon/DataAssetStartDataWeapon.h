// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Data/DataAssetStartDataBase.h"
#include "DataAssetStartDataWeapon.generated.h"

class AKitsuneCharacter;
/**
 * 
 */
UCLASS()
class KITSUNE_API UDataAssetStartDataWeapon : public UDataAssetStartDataBase
{
	GENERATED_BODY()

public:
	void ModifyCharacterData(AKitsuneCharacter* KitsuneCharacter);
	void ResetCharacterData(AKitsuneCharacter* KitsuneCharacter);

	void GiveAbilitiesToASC(UAbilitySystemComponent* TargetASC, int32 AbilityLevel);
	void ClearAbilitiesFromASC(UAbilitySystemComponent* TargetASC) const;

	void GiveWeaponInitialAbilityToASC(UAbilitySystemComponent* TargetASC) const;
	void RemoveWeaponInitialAbilityFromASC(UAbilitySystemComponent* TargetASC) const;

protected:
	/*** `@BC`   描述: 拾取或携带武器后初始能力   `BC@` ***/
	UPROPERTY(EditDefaultsOnly,Category = "Ability", meta = (ForceInlineRow, Categories = "InputTag.Weapon"))
	FAbilityInfo InitialCarriedAbility;
};
