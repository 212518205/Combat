// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Combat/PlayerCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "MotionWarpingComponent.h"
#include "Characters/EnemyCharacter.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "Net/UnrealNetwork.h"
#include "UI/ViewModel/PlayerViewModel.h"


UPlayerCombatComponent::UPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UPlayerCombatComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const APawn* OwnerPawn = GetOwningPawn();
	if (!OwnerPawn)
	{
		return;
	}

	if (OwnerPawn->HasAuthority() && CurrentLockedActor && !IsValid(CurrentLockedActor))
	{
		SetCurrentLockedTarget(nullptr);
		return;
	}

	if (!OwnerPawn->IsLocallyControlled())
	{
		return;
	}

	UpdateViewSnap(DeltaTime);
}

void UPlayerCombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UPlayerCombatComponent, CurrentLockedActor);
}


void UPlayerCombatComponent::SwitchLockedTarget_Implementation()
{
	LockableActors.RemoveAll([](const AActor* Actor) { return !IsValid(Actor); });
	
	int32 Index = INDEX_NONE;
	LockableActors.Find(CurrentLockedActor, Index);
	if (Index != INDEX_NONE)
	{
		int32 NextIndex = (Index + 1) % LockableActors.Num();
		while (NextIndex != Index)
		{
			if (AActor* Current = LockableActors[NextIndex])
			{
				SetCurrentLockedTarget(Current);
				return;
			}
			NextIndex = (NextIndex + 1) % LockableActors.Num();
		}
		SetCurrentLockedTarget(nullptr);
	}
	else
	{
		if (LockableActors.Num() >= 1)
		{
			SetCurrentLockedTarget(LockableActors[0]);
		}
	}
}

void UPlayerCombatComponent::LockedTargetInto_Implementation(AActor* InActor)
{
	if (IsValid(InActor) && !LockableActors.Contains(InActor))
	{
		LockableActors.Add(InActor);
	}
}

void UPlayerCombatComponent::LockedTargetLeave_Implementation(AActor* InActor)
{
	LockableActors.Remove(InActor);
	
	if (CurrentLockedActor == InActor)
	{
		SetCurrentLockedTarget(nullptr);
	}
}

void UPlayerCombatComponent::ClearLockTarget_Implementation()
{
	SetCurrentLockedTarget(nullptr);
}

void UPlayerCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	Super::OnHitTargetActor(HitActor);
	
	if (HitActor == GetOwningPawn())return;
	if (OverlappedActors.Contains(HitActor))return;
	OverlappedActors.AddUnique(HitActor);

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		KitsuneGameplayTags::Shared_Event_MeleeHit,
		Data
	);

}

void UPlayerCombatComponent::OnRep_CurrentLockedActor(const AActor* OldValue)
{
	const APawn* OwnerPawn = GetOwningPawn(); 
	if (!OwnerPawn || !OwnerPawn->IsLocallyControlled())
	{
		return;
	}
	
	if (ILockableInterface* Enemy = Cast<ILockableInterface>(const_cast<AActor*>(OldValue)))
	{
		Enemy->SetLockMarkerVisible(false);
	}
	
	if (ILockableInterface* Enemy = Cast<ILockableInterface>(CurrentLockedActor))
	{
		Enemy->SetLockMarkerVisible(true);
	}
	
	if (CurrentLockedActor && IsValid(CurrentLockedActor))
	{
		if (const AController* Controller = OwnerPawn->GetController())
		{
			if (const FVector ToTarget = CurrentLockedActor->GetActorLocation() - OwnerPawn->GetActorLocation(); !ToTarget.IsNearlyZero())
			{
				const float TargetYaw = ToTarget.Rotation().Yaw;
				if (const float DeltaYaw = FRotator::NormalizeAxis(TargetYaw - Controller->GetControlRotation().Yaw); FMath::Abs(DeltaYaw) > LockViewSnapAngle)
				{
					const float RotateAmount = FMath::Abs(DeltaYaw) - LockViewSnapAngle;
					SnapTargetYaw = Controller->GetControlRotation().Yaw + FMath::Sign(DeltaYaw) * RotateAmount;
					bSnappingToTarget = true;
				}
			}
		}
	}
	
	SetComponentTickEnabled(CurrentLockedActor != nullptr);	
}

FRotator UPlayerCombatComponent::GetViewRotation() const
{
	return GetOwningPawn()->GetControlRotation();
}

bool UPlayerCombatComponent::AddWarpTargetToLockedTarget(const FName WarpTargetName) const
{
	UMotionWarpingComponent* WarpComponent = GetOwningPawn()->FindComponentByClass<UMotionWarpingComponent>();
	if (!WarpComponent || !WarpTargetName.IsValid())return false;
	WarpComponent->RemoveAllWarpTargets();
	
	FTransform TargetTransform;
	if (CurrentLockedActor)TargetTransform = CurrentLockedActor->GetActorTransform();
	else
	{
		FVector CurrentLocation = GetOwningPawn()->GetActorLocation();
		FRotator ViewRotation = GetViewRotation();
		TargetTransform = FTransform(ViewRotation, CurrentLocation);
	}
	
	WarpComponent->AddOrUpdateWarpTargetFromTransform(WarpTargetName, TargetTransform);
	return true;
}

void UPlayerCombatComponent::SetCurrentLockedTarget(AActor* NewTarget)
{
	SetReplicatedProperty(this, CurrentLockedActor, NewTarget, &ThisClass::OnRep_CurrentLockedActor);
	SetComponentTickEnabled(NewTarget != nullptr);
}

void UPlayerCombatComponent::UpdateViewSnap(const float DeltaTime)
{
	if (!bSnappingToTarget)
	{
		return;
	}

	AController* Controller = GetOwningController<AController>();
	if (!Controller)
	{
		bSnappingToTarget = false;
		return;
	}

	FRotator ControlRotation = Controller->GetControlRotation();
	ControlRotation.Yaw = FMath::FixedTurn(ControlRotation.Yaw, SnapTargetYaw, LockViewSnapSpeed * DeltaTime);
	Controller->SetControlRotation(ControlRotation);

	if (FMath::Abs(FRotator::NormalizeAxis(SnapTargetYaw - ControlRotation.Yaw)) < 0.5f)
	{
		bSnappingToTarget = false;
	}
}


