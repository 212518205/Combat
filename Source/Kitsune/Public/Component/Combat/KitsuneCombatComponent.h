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

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* FindWeaponByTag(const FGameplayTag& WeaponTag) const;


	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetCurrentCarriedWeapon() const;

	/*** `@BC`   描述: 设置当前武器标签后需要将武器对应的技能赋予角色   `BC@` ***/
	UFUNCTION(BlueprintCallable)
	AWeaponBase* SetCurrentWeapon(const FGameplayTag& WeaponTag);

protected:
	/*** `@BC`   描述: 角色已拥有武器   `BC@` ***/
	TMap<FGameplayTag, TObjectPtr<AWeaponBase>> CarriedWeaponMap;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	FGameplayTag CurrentWeaponTag = FGameplayTag();

};
