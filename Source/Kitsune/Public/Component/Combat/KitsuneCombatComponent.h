// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Component/KitsuneExtensionComponent.h"
#include "KitsuneCombatComponent.generated.h"

class AWeaponBase;

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneCombatComponent : public UKitsuneExtensionComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void RegisterCarriedWeapon(const FGameplayTag InWeaponTag, AWeaponBase* NewWeapon, const bool bRegisterAndEquip = false);

	TObjectPtr<AWeaponBase> FindWeaponByTag(const FGameplayTag& WeaponTag) const;
	TObjectPtr<AWeaponBase> GetCurrentCarriedWeapon() const;

	FGameplayTag CurrentWeaponTag = FGameplayTag();

protected:
	/*** `@BC`   描述: 角色已拥有武器   `BC@` ***/
	TMap<FGameplayTag, TObjectPtr<AWeaponBase>> CarriedWeaponMap;

};
