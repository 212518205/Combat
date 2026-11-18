#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Component/KitsuneExtensionComponent.h"
#include "KitsuneCombatComponent.generated.h"

class AWeaponBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentWeaponChangedDelegate, AWeaponBase*);

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneCombatComponent : public UKitsuneExtensionComponent
{
	GENERATED_BODY()

public:
	UKitsuneCombatComponent();

	virtual void BeginPlay() override;

	void EnsureCombatViewModel();

	UFUNCTION(Server, Reliable,  BlueprintCallable, Category = "Weapon")
	void RegisterCarriedWeapon(AWeaponBase* NewWeapon, const bool bRegisterAndEquip = false);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* FindWeaponByTag(const FGameplayTag& WeaponTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetCurrentCarriedWeapon() const;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetCurrentWeapon(const FGameplayTag& WeaponTag);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ToggleWeaponCollision(bool bEnable);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FOnCurrentWeaponChangedDelegate OnCurrentWeaponChanged;

protected:
	/*** `@BC`   描述: 角色已拥有武器   `BC@` ***/
	UPROPERTY(Replicated)
	TArray<TObjectPtr<AWeaponBase>> CarriedWeapons;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeaponTag, BlueprintReadWrite, Category = "Weapon")
	FGameplayTag CurrentWeaponTag = FGameplayTag();

	UFUNCTION()
	void OnRep_CurrentWeaponTag(const FGameplayTag& OldValue);

	TArray<TObjectPtr<AActor>> OverlappedActors;

	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnPulledFromTargetActor(AActor* InteractedActor);

};






