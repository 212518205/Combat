// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KitsuneFunctionLibrary.generated.h"

class UKitsuneAbilitySystemComponent;
class UKitsuneCombatComponent;
/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UKitsuneCombatComponent* NativeGetKitsuneCombatComponentFromActor(AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary",
		meta = (DisplayName = "Get Kitsune Combate Component", ExpandEnumAsExecs = "OutValidType"))
	static UKitsuneCombatComponent* BP_GetKitsuneCombatComponentFromActor(AActor* Actor, EKitsuneValidType& OutValidType);

	static UKitsuneAbilitySystemComponent* NativeGetKitsuneASCFromActor(AActor* OwningActor);
	UFUNCTION(BlueprintPure, Category = "FunctionLibrary",
		meta = (DisplayName = "Get Kitsune Ability System Component", ExpandEnumAsExecs = "OutValidType"))
	static UKitsuneAbilitySystemComponent* BP_GetKitsuneAbilitySystemComponentFromActor(
		AActor* OwningActor, EKitsuneValidType& OutValidType);

	static bool NativeCheckHitResult(const float HitChance);
	UFUNCTION(BlueprintPure, Category = "FunctionLibrary",
		meta = (DisplayName = "Check Chance Hit Result", ExpandBoolAsExecs = "OutHitResult"))
	static void BP_CheckHitResult(const float HitChance, bool& OutHitResult);

};
