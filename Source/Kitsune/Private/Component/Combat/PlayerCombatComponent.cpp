// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Combat/PlayerCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "UIManagerSubsystem.h"
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

	UpdateLockedTargetRotation(DeltaTime);
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
	if (const APawn* OwnerPawn = GetOwningPawn(); !OwnerPawn || !OwnerPawn->IsLocallyControlled())
	{
		return;
	}
	
	if (UPlayerViewModel* PlayerVM = UUIManagerSubsystem::GetUIManager(GetOwner())->TryGetViewModelByActor<UPlayerViewModel>(GetOwningPawn()))
	{
		PlayerVM->SetHasLockedTarget(true);
	}
	/*** TODO: 转向这个目标... [2026年8月27日 0:18:20 来自`@BC@`] ***/
	SetComponentTickEnabled(CurrentLockedActor != nullptr);	
}

void UPlayerCombatComponent::SetCurrentLockedTarget(AActor* NewTarget)
{
	SetReplicatedProperty(this, CurrentLockedActor, NewTarget, &ThisClass::OnRep_CurrentLockedActor);
	SetComponentTickEnabled(NewTarget != nullptr);
}

void UPlayerCombatComponent::UpdateLockedTargetRotation(const float DeltaTime) const
{
	AController* Controller = GetOwningController<AController>();
	if (!Controller || !IsValid(CurrentLockedActor))
	{
		return;
	}

	const FVector ToTarget = CurrentLockedActor->GetActorLocation() - GetOwningPawn()->GetActorLocation();
	if (ToTarget.IsNearlyZero())
	{
		return;
	}

	const FRotator TargetRotation = ToTarget.Rotation();
	FRotator ControlRotation = Controller->GetControlRotation();
	ControlRotation.Yaw = FMath::FixedTurn(ControlRotation.Yaw, TargetRotation.Yaw, LockRotationSpeed * DeltaTime);
	Controller->SetControlRotation(ControlRotation);
}

