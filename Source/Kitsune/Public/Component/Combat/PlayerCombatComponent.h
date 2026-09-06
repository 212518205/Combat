// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Combat/KitsuneCombatComponent.h"
#include "PlayerCombatComponent.generated.h"

class UMotionWarpingComponent;
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
	
	UFUNCTION(Server, Reliable)
	void ClearLockTarget();

	AActor* GetCurrentLockActor() const { return CurrentLockedActor; }

protected:
	virtual void OnHitTargetActor(AActor* HitActor) override;
	
	UPROPERTY()
	TArray<AActor*> LockableActors;
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentLockedActor, BlueprintReadOnly)
	TObjectPtr<AActor> CurrentLockedActor;
	
	UFUNCTION()
	void OnRep_CurrentLockedActor(const AActor* OldValue);

	FRotator GetViewRotation() const;
	
private:
	void SetCurrentLockedTarget(AActor* NewTarget);
	
	void UpdateViewSnap(float DeltaTime);

	UPROPERTY(EditDefaultsOnly, Category = "Combat | Lock")
	float LockViewSnapAngle = 60.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat | Lock")
	float LockViewSnapSpeed = 540.f;

	// 【新增】一次性对齐状态（非 UPROPERTY，仅运行期短暂使用）
	bool bSnappingToTarget = false;
	float SnapTargetYaw = 0.f;
	
};
