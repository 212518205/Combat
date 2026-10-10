// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "EffectActor.generated.h"

class UAbilitySystemComponent;
class USphereComponent;

UENUM(BlueprintType)
enum class EGameplayEffectApplyPolicy: uint8
{
	Default UMETA(DisplayName = "Default"),
	OverlapBegin UMETA(DisplayName="OverlapBegin"),
	OverlapEnd UMETA(DisplayName = "OverlapEnd")
};

UENUM(BlueprintType)
enum class EGameplayEffectRemovePolicy:uint8
{
	Default UMETA(DisplayName="Default"),
	OverlapBegin UMETA(DisplayName="OverlapBegin"),
	OverlapEnd UMETA(DisplayName = "OverlapEnd")
};

UCLASS()
class KITSUNE_API AEffectActor : public AActor
{
	GENERATED_BODY()
	
public:
	/*Public Function Begin*/
	AEffectActor();
	/*Public Function End*/

protected:
	/*Protected Function Begin*/
	UFUNCTION(BlueprintCallable,Category = "Effect | Apply")
	void ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GEClass);

	UFUNCTION(BlueprintCallable,Category = "Effect | Apply")
	void OverlapBegin(AActor* TargetActor);

	UFUNCTION(BlueprintCallable,Category = "Effect | Apply")
	void OverlapEnd(AActor* TargetActor);
	/*Protected Function End*/

	/*Protected Variable Begin*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effect | Class")         
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effect | Class")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effect | Class")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effect | Policy | Apply")
	EGameplayEffectApplyPolicy InstantGameplayEffectApplyPolicy = EGameplayEffectApplyPolicy::Default;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect | Policy | Apply")
	EGameplayEffectApplyPolicy DurationGameplayEffectApplyPolicy = EGameplayEffectApplyPolicy::Default;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect | Policy | Apply")
	EGameplayEffectApplyPolicy InfiniteGameplayEffectApplyPolicy = EGameplayEffectApplyPolicy::Default;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effect | Policy | Remove")
	EGameplayEffectRemovePolicy InfiniteGameplayEffectRemovePolicy = EGameplayEffectRemovePolicy::Default;
	UPROPERTY(BlueprintReadOnly,Category = "Effect | RemoveHandles")
	TMap<FActiveGameplayEffectHandle,UAbilitySystemComponent*> RemoveActiveEffectHandle;

	UPROPERTY(EditAnywhere,Category = "Effect | Level")
	float Level = 1.f;
	/*Protected Variable End*/
};
