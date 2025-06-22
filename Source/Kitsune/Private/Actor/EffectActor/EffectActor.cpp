// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EffectActor/EffectActor.h"

#include "AbilitySyetem/KitsuneAttributeSet.h"
#include "Characters/KitsuneCharacter.h"
#include"AbilitySystemBlueprintLibrary.h"

// Sets default values
AEffectActor::AEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(Scene);
}

void AEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GEClass)
{
	UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetAsc)return;
	checkf(GEClass, TEXT("GEClass is NULL"));
	FGameplayEffectContextHandle EffectContextHandle = TargetAsc->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAsc->MakeOutgoingSpec(GEClass, 1.0, EffectContextHandle);
	TargetAsc->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}


