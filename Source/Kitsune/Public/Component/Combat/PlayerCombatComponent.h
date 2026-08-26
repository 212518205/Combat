// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Combat/KitsuneCombatComponent.h"
#include "PlayerCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UPlayerCombatComponent : public UKitsuneCombatComponent
{
	GENERATED_BODY()
	
public:
	UPlayerCombatComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Server, Reliable)
	void SwitchLockedTarget();
	
	UFUNCTION(Server, Reliable)
	void LockedTargetInto(AActor* InActor);
	
	UFUNCTION(Server, Reliable)
	void LockedTargetLeave(AActor* InActor);

protected:
	virtual void OnHitTargetActor(AActor* HitActor) override;
	
	UPROPERTY()
	TArray<AActor*> LockableActors;
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentLockedActor)
	TObjectPtr<AActor> CurrentLockedActor;
	
	UFUNCTION()
	void OnRep_CurrentLockedActor(const AActor* OldValue);
	
private:
	void SetCurrentLockedTarget(AActor* NewTarget);

	void UpdateLockedTargetRotation(float DeltaTime) const;

	/*** `@BC`   描述: 锁定转向速度（度/秒）   `BC@` ***/
	UPROPERTY(EditDefaultsOnly, Category = "Combat | Lock")
	float LockRotationSpeed = 360.f;
	
};
