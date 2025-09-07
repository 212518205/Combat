// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySyetem/KitsuneAbilitySystemComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	PrimaryActorTick.bCanEverTick = false;

}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ACharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::InitAbilityInfo()
{
}

void ACharacterBase::InitializeAttributes() const
{
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultAttributes, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACharacterBase::AddInitialAbility() const
{
	if (!HasAuthority())return;

	UKitsuneAbilitySystemComponent* KitsuneAbilitySystemComponent = Cast<UKitsuneAbilitySystemComponent>(AbilitySystemComponent);
	KitsuneAbilitySystemComponent->AddInitialAbilities(CharacterAbilities);
}


