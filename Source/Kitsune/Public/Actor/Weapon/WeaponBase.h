// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Actor/ItemBase.h"
#include "WeaponBase.generated.h"


class ACharacterBase;
class UDataAssetStartDataWeapon;
class UKitsuneAbilitySystemComponent;
class UBoxComponent;

DECLARE_DELEGATE_OneParam(FOnWeaponOverlapChangedDelegate, AActor*);

/**
 * 
 */
UCLASS()
class KITSUNE_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

public:
	AWeaponBase();

	void EquipWeaponToCharacter(ACharacterBase* TargetCharacter) const;
	void UnequipWeaponFromCharacter(ACharacterBase* TargetCharacter) const;

	void GiveWeaponInitialAbilityToASC(UAbilitySystemComponent* TargetASC) const;

	FORCEINLINE UBoxComponent* GetWeaponBoxCollision() { return ItemOverlapBox; }
	FORCEINLINE UDataAssetStartDataWeapon* GetWeaponInfo() const { return WeaponDataInfo; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FGameplayTag WeaponTag;

	FOnWeaponOverlapChangedDelegate WeaponBeginOverlap;
	FOnWeaponOverlapChangedDelegate WeaponEndOverlap;

protected:
	/*** `@BC`   描述: 	基类基础上添加盒体检测   `BC@` ***/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UBoxComponent> ItemOverlapBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Ability System")
	TObjectPtr<UDataAssetStartDataWeapon> WeaponDataInfo;

	/*** `@BC`   描述: 重叠开始结束回调   `BC@` ***/
	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	int32 WeaponLevel = 1;

};
