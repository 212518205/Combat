// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendDebugHelper.h"
#include "GameplayTagContainer.h"
#include "Component/KitsuneExtensionComponent.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KitsuneFunctionLibrary.generated.h"

class UKitsuneExtensionComponent;
class UKitsuneAbilitySystemComponent;
class UKitsuneCombatComponent;

namespace KitsuneNet
{
	template<typename ObjectType, typename PropertyType>
	FORCEINLINE bool SetReplicatedProperty(ObjectType* Object, PropertyType& Property, const PropertyType& NewValue, void (ObjectType::*OnRep)() = nullptr)
	{
		if (!IsValid(Object))
		{
			Debug::Print(TEXT("SetReplicatedProperty: Object 无效"));
			return false;
		}

		const APawn* Pawn = nullptr;
		if (const UKitsuneExtensionComponent* Owner = Cast<UKitsuneExtensionComponent>(Object))
		{
			Pawn = Owner->GetOwningPawn();
		}else
		{
			Debug::Print(TEXT("Object 不是 KitsuneExtensionComponent!"));
			return false;
		}

		if (!Pawn->HasAuthority())
		{
			Debug::Print(TEXT("SetReplicatedProperty: 客户端尝试设置"));
				return false;
		}
		Property = NewValue;
		if (UWorld* World = Pawn->GetWorld();World && OnRep)
		{
			if (const ENetMode NetMode = World->GetNetMode(); NetMode == NM_ListenServer || NetMode == NM_Standalone)
			{
				if (NetMode != NM_ListenServer || Pawn->IsLocallyControlled())
				{
					(Object->*OnRep)();
				}
			}
		}
		return true;
	}
}

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*** `@BC`   描述: Actor 所携带GameplayTag的添加删除查询   `BC@` ***/
	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* TargetActor, const FGameplayTag ActorTag);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void TryRemoveGameplayTagFromActor(AActor* TargetActor, const FGameplayTag ActorTag);

	static bool NativeDoesActorHaveTag(AActor* TargetActor, const FGameplayTag CheckedTag);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandBoolAsExecs = "OutCheckResult"))
	static bool BP_DoesActorHaveTag(AActor* TargetActor, const FGameplayTag CheckedTag, bool& OutCheckResult);

	/*** `@BC`   描述: Get Component API   `BC@` ***/
	static UKitsuneCombatComponent* NativeGetKitsuneCombatComponentFromActor(AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary",
		meta = (DisplayName = "Get Kitsune Combate Component", ExpandEnumAsExecs = "OutValidType"))
	static UKitsuneCombatComponent* BP_GetKitsuneCombatComponentFromActor(AActor* Actor, EKitsuneValidType& OutValidType);

	static UKitsuneAbilitySystemComponent* NativeGetKitsuneASCFromActor(AActor* OwningActor);
	UFUNCTION(BlueprintPure, Category = "FunctionLibrary",
		meta = (DisplayName = "Get Kitsune Ability System Component", ExpandEnumAsExecs = "OutValidType"))
	static UKitsuneAbilitySystemComponent* BP_GetKitsuneAbilitySystemComponentFromActor(
		AActor* OwningActor, EKitsuneValidType& OutValidType);

	/*** `@BC`   描述: 辅助函数   `BC@` ***/
	static bool NativeCheckHitResult(const float HitChance);
	UFUNCTION(BlueprintPure, Category = "FunctionLibrary",
		meta = (DisplayName = "Check Chance Hit Result", ExpandBoolAsExecs = "OutHitResult"))
	static void BP_CheckHitResult(const float HitChance, bool& OutHitResult);

	UFUNCTION(BlueprintPure, Category = "FunctionLibrary|")
	static bool IsHostileToPawn(const APawn* QueryPawn, const APawn* TargetPawn);

};
