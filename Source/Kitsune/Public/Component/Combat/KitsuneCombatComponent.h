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
	UKitsuneCombatComponent();

	UFUNCTION(Server, Reliable,  BlueprintCallable, Category = "Weapon")
	void RegisterCarriedWeapon(AWeaponBase* NewWeapon, const bool bRegisterAndEquip = false);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* FindWeaponByTag(const FGameplayTag& WeaponTag) const;


	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetCurrentCarriedWeapon() const;

	/*** `@BC`   描述: 设置当前武器标签后需要将武器对应的技能赋予角色   `BC@` ***/
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetCurrentWeapon(const FGameplayTag& WeaponTag);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ToggleWeaponCollision(bool bEnable);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	/*** `@BC`   描述: 角色已拥有武器   `BC@` ***/
	UPROPERTY(Replicated)
	TArray<TObjectPtr<AWeaponBase>> CarriedWeapons;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeaponTag, BlueprintReadWrite, Category = "Weapon")
	FGameplayTag CurrentWeaponTag = FGameplayTag();

	UFUNCTION()
	void OnRep_CurrentWeaponTag();

	void SwitchWeaponIcon();


	TArray<TObjectPtr<AActor>> OverlappedActors;


protected:
	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnPulledFromTargetActor(AActor* InteractedActor);

};






