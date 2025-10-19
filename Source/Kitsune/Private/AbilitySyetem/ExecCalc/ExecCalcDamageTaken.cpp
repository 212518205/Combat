// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySyetem/ExecCalc/ExecCalcDamageTaken.h"

#include "FrontendDebugHelper.h"
#include "AbilitySyetem/KitsuneAttributeSet.h"
#include "FunctionLibrary/KitsuneFunctionLibrary.h"

struct FKitsuneDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DodgeChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)

	FKitsuneDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKitsuneAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKitsuneAttributeSet, CriticalChance, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKitsuneAttributeSet, CriticalDamage, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKitsuneAttributeSet, DodgeChance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKitsuneAttributeSet, Health, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKitsuneAttributeSet, Armor, Target, false)
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
}

void UExecCalcDamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                  FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float FinalDamage = 0.f;

	ATTRIBUTE_GETTER(DodgeChance, Target);

	ATTRIBUTE_GETTER(Health, Target);

	if (UKitsuneFunctionLibrary::NativeCheckHitResult(TargetDodgeChance))
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				DamageCapture().HealthProperty,
				EGameplayModOp::Override,
				FinalDamage
				)
		);
	}

	ATTRIBUTE_GETTER(AttackPower, Source);

	ATTRIBUTE_GETTER(CriticalChance, Source);
	if (UKitsuneFunctionLibrary::NativeCheckHitResult(SourceCriticalChance)) {
		SourceAttackPower *= 2;
		ATTRIBUTE_GETTER(CriticalDamage, Source);
		SourceAttackPower *= SourceCriticalDamage;
	}
	ATTRIBUTE_GETTER(Armor, Target);
	SourceAttackPower -= TargetArmor;

	Debug::Print(TEXT("Attack Damage"), SourceAttackPower);

	TargetHealth -= SourceAttackPower;
	if (TargetHealth >= 0)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				DamageCapture().HealthProperty,
				EGameplayModOp::Override,
				TargetHealth
			)
		);
	}

	Debug::Print(TEXT("Hit Actor Final Health"), TargetHealth);

}
