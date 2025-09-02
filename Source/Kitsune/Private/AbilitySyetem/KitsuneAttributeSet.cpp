// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySyetem/KitsuneAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include"Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"


void UKitsuneAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always)

	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, SpiritWisdom, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, MaxSpiritWisdom, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, MaxVigor, COND_None, REPNOTIFY_Always);

	// 战斗属性（Combat Attribute）复制配置
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, Toughness, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, MentalFocus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, Armor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, CriticalStrikeChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, CriticalStrikeMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, MentalCriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, MentalCriticalMultiplier, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, MentalBurn, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, DodgeChance, COND_None, REPNOTIFY_Always);
}

void UKitsuneAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute==GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}

void UKitsuneAttributeSet::PostGameplayEffectExecute(const  FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


	FEffectProperties Props;
	SetEffectProperties(Data, Props);
}

void UKitsuneAttributeSet::SetEffectProperties(const struct FGameplayEffectModCallbackData& Data,
	FEffectProperties& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() 
		&& Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController==nullptr && Props.SourceASC->AbilityActorInfo->AvatarActor.Get())
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}

		if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
		{
			Props.TargetAvatarActor = Data.Target.GetAvatarActor();
			Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
			Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
			Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
		}
	}
}

void UKitsuneAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, Health, OldHealth);
}

void UKitsuneAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, MaxHealth, OldMaxHealth);
}

void UKitsuneAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, Stamina, OldStamina);
}

void UKitsuneAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, MaxStamina, OldMaxStamina);
}

void UKitsuneAttributeSet::OnRep_SpiritWisdom(const FGameplayAttributeData& OldSpiritWisdom) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, SpiritWisdom, OldSpiritWisdom);
}

void UKitsuneAttributeSet::OnRep_MaxSpiritWisdom(const FGameplayAttributeData& OldMaxSpiritWisdom) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, MaxSpiritWisdom, OldMaxSpiritWisdom);
}

void UKitsuneAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, Vigor, OldVigor);
}

void UKitsuneAttributeSet::OnRep_MaxVigor(const FGameplayAttributeData& OldMaxVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, MaxVigor, OldMaxVigor);
}

void UKitsuneAttributeSet::OnRep_Toughness(const FGameplayAttributeData& OldToughness) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, Toughness, OldToughness);
}

void UKitsuneAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, Shield, OldShield);
}

void UKitsuneAttributeSet::OnRep_MentalFocus(const FGameplayAttributeData& OldMentalFocus) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, MentalFocus, OldMentalFocus);
}

void UKitsuneAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, Armor, OldArmor);
}

void UKitsuneAttributeSet::OnRep_CriticalStrikeChance(const FGameplayAttributeData& OldCriticalStrikeChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, CriticalStrikeChance, OldCriticalStrikeChance);
}

void UKitsuneAttributeSet::OnRep_CriticalStrikeMultiplier(
	const FGameplayAttributeData& OldCriticalStrikeMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, CriticalStrikeMultiplier, OldCriticalStrikeMultiplier);
}

void UKitsuneAttributeSet::OnRep_MentalCriticalChance(const FGameplayAttributeData& OldMentalCriticalChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, MentalCriticalChance, OldMentalCriticalChance);
}

void UKitsuneAttributeSet::OnRep_MentalCriticalMultiplier(
	const FGameplayAttributeData& OldMentalCriticalMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, MentalCriticalMultiplier, OldMentalCriticalMultiplier);
}

void UKitsuneAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UKitsuneAttributeSet::OnRep_MentalBurn(const FGameplayAttributeData& OldMentalBurn) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, MentalBurn, OldMentalBurn);
}

void UKitsuneAttributeSet::OnRep_DodgeChance(const FGameplayAttributeData& OldDodgeChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, DodgeChance, OldDodgeChance);
}
