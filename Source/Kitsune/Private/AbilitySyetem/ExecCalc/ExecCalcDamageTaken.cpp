// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySyetem/ExecCalc/ExecCalcDamageTaken.h"

#include "FrontendDebugHelper.h"
#include "AbilitySyetem/KitsuneAttributeSet.h"
#include "FunctionLibrary/KitsuneFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"

struct FKitsuneDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DodgeChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FKitsuneDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKitsuneAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKitsuneAttributeSet, CriticalChance, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKitsuneAttributeSet, CriticalDamage, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKitsuneAttributeSet, DodgeChance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKitsuneAttributeSet, Health, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKitsuneAttributeSet, Armor, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKitsuneAttributeSet, DamageTaken, Target, false)
	}
};

namespace{
	const FKitsuneDamageCapture& DamageCapture()
	{
		static FKitsuneDamageCapture KitsuneDamageCapture;
		return KitsuneDamageCapture;
	}

#define ATTRIBUTE_GETTER(Name, T) \
	float T##Name = 0.f;\
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageCapture().Name##Def, EvaluateParameters,\
		T##Name)

}

UExecCalcDamageTaken::UExecCalcDamageTaken()
{
	RelevantAttributesToCapture.Add(DamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(DamageCapture().CriticalChanceDef);
	RelevantAttributesToCapture.Add(DamageCapture().CriticalDamageDef);
	RelevantAttributesToCapture.Add(DamageCapture().DodgeChanceDef);
	RelevantAttributesToCapture.Add(DamageCapture().HealthDef);
	RelevantAttributesToCapture.Add(DamageCapture().ArmorDef);
	RelevantAttributesToCapture.Add(DamageCapture().DamageTakenDef);
}

void UExecCalcDamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                  FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	ATTRIBUTE_GETTER(DodgeChance, Target);
	ATTRIBUTE_GETTER(Health, Target);

	if (UKitsuneFunctionLibrary::NativeCheckHitResult(TargetDodgeChance))
	{
		/*** TODO: 伤害触发闪避，通知UI显示闪避效果... [2025年10月25日 23:36:01 来自`@BC@`] ***/
		Debug::Print(TEXT("触发闪避"));
	}

	ATTRIBUTE_GETTER(AttackPower, Source);
	ATTRIBUTE_GETTER(CriticalChance, Source);
	ATTRIBUTE_GETTER(Armor, Target);
	ATTRIBUTE_GETTER(CriticalDamage, Source);

	for (auto [BonusTag, BonusMultiplier] : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (BonusTag == KitsuneGameplayTags::Shared_SetByCaller_DamageBonusMultiplier) {
			SourceAttackPower *= BonusMultiplier;
		}
	}

	if (UKitsuneFunctionLibrary::NativeCheckHitResult(SourceCriticalChance)) {
		SourceAttackPower *= 2;
		SourceAttackPower *= SourceCriticalDamage;
	}
	SourceAttackPower -= TargetArmor;


	if (SourceAttackPower >= 0)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				DamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				SourceAttackPower
			)
		);
	}

}
