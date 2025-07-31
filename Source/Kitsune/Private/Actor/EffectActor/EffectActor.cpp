// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EffectActor/EffectActor.h"

#include "AbilitySyetem/KitsuneAttributeSet.h"
#include"AbilitySystemBlueprintLibrary.h"

// Sets default values
AEffectActor::AEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(Scene);
}

void AEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GEClass)
{
	UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetAsc)return;
	checkf(GEClass, TEXT("GEClass is NULL"));
	FGameplayEffectContextHandle EffectContextHandle = TargetAsc->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAsc->MakeOutgoingSpec(GEClass, Level, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = TargetAsc->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (InfiniteGameplayEffectRemovePolicy==EGameplayEffectRemovePolicy::EOverlapEnd&&bIsInfinite)
	{
		RemoveActiveEffectHandle.Add(ActiveEffectHandle, TargetAsc);
	}
}

void AEffectActor::OverlapBegin(AActor* TargetActor)
{
	if (InstantGameplayEffectApplyPolicy==EGameplayEffectApplyPolicy::EOverlapBegin)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationGameplayEffectApplyPolicy == EGameplayEffectApplyPolicy::EOverlapBegin)
	{
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
	if (InfiniteGameplayEffectApplyPolicy == EGameplayEffectApplyPolicy::EOverlapBegin)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AEffectActor::OverlapEnd(AActor* TargetActor)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (IsValid(TargetASC))
	{
		FActiveGameplayEffectHandle ActiveEffectHandle;
		for (auto& Pair:RemoveActiveEffectHandle)
		{
			if (Pair.Value==TargetASC)
			{
				ActiveEffectHandle = Pair.Key;
				break;
			}
		}
		RemoveActiveEffectHandle.Remove(ActiveEffectHandle);
		TargetASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
	}
}


