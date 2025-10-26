// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySyetem/KitsuneAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FrontendDebugHelper.h"
#include"Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "FunctionLibrary/KitsuneFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayTag/KitsuneGameplayTag.h"


void UKitsuneAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always)

	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, MaxVigor, COND_None, REPNOTIFY_Always);

	/*** `@BC`   描述: 战斗资源   `BC@` ***/
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, SpiritPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);

	// 战斗属性（Combat Attribute）复制配置
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, Toughness, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, MentalFocus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, Armor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, SoulCritical, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, SoulCriticalDamage, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, SoulFlare, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, DodgeChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, DamageTaken, COND_None, REPNOTIFY_Always);
}

void UKitsuneAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	
}

void UKitsuneAttributeSet::PostGameplayEffectExecute(const  FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float CurrentHealth = FMath::Clamp(GetHealth(), 0.f, GetMaxHealth());
		SetHealth(CurrentHealth);
	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		const float CurrentStamina = FMath::Clamp(GetStamina(), 0.f, GetMaxStamina());
		SetStamina(CurrentStamina);
	}

	if (Data.EvaluatedData.Attribute == GetVigorAttribute())
	{
		const float CurrentVigor = FMath::Clamp(GetVigor(), 0.f, GetMaxVigor());
		SetVigor(CurrentVigor);
	}


	if (const float a = GetDamageTaken(); Data.EvaluatedData.Attribute == GetDamageTakenAttribute() && a !=0.f)
	{
		const float CurrentDamageTaken = GetDamageTaken();
		if (CurrentDamageTaken <= 0)
		{
			/*** TODO: 伤害值不足以击穿护甲，将造成1的伤害... [2025年10月25日 23:32:58 来自`@BC@`] ***/
		}
		const float CurrentHealth = GetHealth();
		const float FinalHealth = FMath::Clamp(CurrentHealth - CurrentDamageTaken, 0.f, GetMaxHealth());
		SetHealth(FinalHealth);
		if ( FinalHealth == 0.f)
		{
			UKitsuneFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), KitsuneGameplayTags::Shared_Status_Dead);
			Debug::Print(TEXT("被打死了"));
		}else
		{
			FGameplayEventData EventData;
			EventData.Instigator = Data.EffectSpec.GetContext().GetInstigator();
			EventData.Target = Data.Target.GetAvatarActor();

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.Target.GetAvatarActor(),
				KitsuneGameplayTags::Shared_Event_HitReact,
				EventData);
		}

	}
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

void UKitsuneAttributeSet::OnRep_SpiritPower(const FGameplayAttributeData& OldSpiritPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, SpiritPower, OldSpiritPower);
}

void UKitsuneAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, AttackPower, OldAttackPower);
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

void UKitsuneAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, CriticalChance, OldCriticalChance);
}

void UKitsuneAttributeSet::OnRep_CriticalDamage(const FGameplayAttributeData& OldCriticalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, CriticalDamage, OldCriticalDamage);
}

void UKitsuneAttributeSet::OnRep_SoulCritical(const FGameplayAttributeData& OldSoulCritical) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, SoulCritical, OldSoulCritical);
}

void UKitsuneAttributeSet::OnRep_SoulCriticalDamage(const FGameplayAttributeData& OldSoulCriticalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, SoulCriticalDamage, OldSoulCriticalDamage);
}

void UKitsuneAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UKitsuneAttributeSet::OnRep_SoulFlare(const FGameplayAttributeData& OldSoulFlare) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, SoulFlare, OldSoulFlare);
}

void UKitsuneAttributeSet::OnRep_DodgeChance(const FGameplayAttributeData& OldDodgeChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, DodgeChance, OldDodgeChance);
}

void UKitsuneAttributeSet::OnRep_DamageTaken(const FGameplayAttributeData& OldDamageTaken) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, DamageTaken, OldDamageTaken);
}


