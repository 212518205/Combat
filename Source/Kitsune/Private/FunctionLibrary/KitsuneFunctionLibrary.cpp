// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/KitsuneFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySyetem/KitsuneAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"

UKitsuneCombatComponent* UKitsuneFunctionLibrary::NativeGetKitsuneCombatComponentFromActor(AActor* Actor)
{
	if (!Actor)return nullptr;
	const IPawnCombatInterface* CombatInterface = Cast<IPawnCombatInterface>(Actor);
	return CombatInterface ? CombatInterface->GetKitsuneCombatComponent() : nullptr;
}

UKitsuneCombatComponent* UKitsuneFunctionLibrary::BP_GetKitsuneCombatComponentFromActor(AActor* Actor,
	EKitsuneValidType& OutValidType)
{
	UKitsuneCombatComponent* KitsuneCombatComponent = NativeGetKitsuneCombatComponentFromActor(Actor);

	OutValidType = KitsuneCombatComponent ? EKitsuneValidType::Valid : EKitsuneValidType::InValid;

	return KitsuneCombatComponent;
}

UKitsuneAbilitySystemComponent* UKitsuneFunctionLibrary::NativeGetKitsuneASCFromActor(AActor* OwningActor)
{
	return Cast<UKitsuneAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningActor));
}

UKitsuneAbilitySystemComponent* UKitsuneFunctionLibrary::BP_GetKitsuneAbilitySystemComponentFromActor(
	AActor* OwningActor, EKitsuneValidType& OutValidType)
{
	UKitsuneAbilitySystemComponent* KitsuneAbilitySystemComponent = NativeGetKitsuneASCFromActor(OwningActor);
	OutValidType = KitsuneAbilitySystemComponent ? EKitsuneValidType::Valid : EKitsuneValidType::InValid;

	return KitsuneAbilitySystemComponent;
}

bool UKitsuneFunctionLibrary::NativeCheckHitResult(const float HitChance)
{
	return FMath::RandRange(1, 100) < HitChance;
}

void UKitsuneFunctionLibrary::BP_CheckHitResult(const float HitChance, bool& OutHitResult)
{
	OutHitResult = NativeCheckHitResult(HitChance);
}
