// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include"AbilitySystemComponent.h"
#include "KitsuneAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)\


USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {};

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()                                                                                                                                      
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;

};

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*** ``   基础资源   `` ***/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attribute|Combat")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet,Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attribute|Combat")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, MaxHealth)

	/*** `@BC`   描述: 耐力值   `BC@` ***/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Attribute|Combat")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Attribute|Combat")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Attribute|Combat")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, Vigor)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxVigor, Category = "Attribute|Combat")
	FGameplayAttributeData MaxVigor;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, MaxVigor)
	/*** ``   基础资源   `` ***/

	/*** ``   战斗资源   `` ***/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackPower, Category = "Attribute|Combat")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SpiritPower, Category = "Attribute|Combat")
	FGameplayAttributeData SpiritPower;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, SpiritPower)
	/*** ``   战斗资源   `` ***/

	/*** ``   战斗属性   `` ***/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Toughness, Category = "Attribute|Combat")
	FGameplayAttributeData Toughness;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, Toughness)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield, Category = "Attribute|Combat")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, Shield)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MentalFocus, Category = "Attribute|Combat")
	FGameplayAttributeData MentalFocus;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, MentalFocus)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Attribute|Combat")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, Armor)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalChance, Category = "Attribute|Combat")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, CriticalChance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalDamage, Category = "Attribute|Combat")
	FGameplayAttributeData CriticalDamage;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, CriticalDamage)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SoulCritical, Category = "Attribute|Combat")
	FGameplayAttributeData SoulCritical;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, SoulCritical)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SoulCriticalDamage, Category = "Attribute|Combat")
	FGameplayAttributeData SoulCriticalDamage;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, SoulCriticalDamage)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Attribute|Combat")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, ArmorPenetration)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SoulFlare, Category = "Attribute|Combat")
	FGameplayAttributeData SoulFlare;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, SoulFlare)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DodgeChance, Category = "Attribute|Combat")
	FGameplayAttributeData DodgeChance;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, DodgeChance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageTaken, Category = "Attribute|Combat")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, DamageTaken)
	/*** ``   战斗属性   `` ***/
protected:
	/*** `@BC`   描述: 修改前修约属性数值   `BC@` ***/
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const  FGameplayEffectModCallbackData& Data) override;

	static void SetEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& Props);


	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	// 耐力系统
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_MaxVigor(const FGameplayAttributeData& OldMaxVigor) const;

	UFUNCTION()
	void OnRep_SpiritPower(const FGameplayAttributeData& OldSpiritPower) const;

	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const;

	UFUNCTION()
	void OnRep_Toughness(const FGameplayAttributeData& OldToughness) const;

	UFUNCTION()
	void OnRep_Shield(const FGameplayAttributeData& OldShield) const;

	UFUNCTION()
	void OnRep_MentalFocus(const FGameplayAttributeData& OldMentalFocus) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const;

	UFUNCTION()
	void OnRep_CriticalDamage(const FGameplayAttributeData& OldCriticalDamage) const;

	UFUNCTION()
	void OnRep_SoulCritical(const FGameplayAttributeData& OldSoulCritical) const;

	UFUNCTION()
	void OnRep_SoulCriticalDamage(const FGameplayAttributeData& OldSoulCriticalDamage) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_SoulFlare(const FGameplayAttributeData& OldSoulFlare) const;

	UFUNCTION()
	void OnRep_DodgeChance(const FGameplayAttributeData& OldDodgeChance) const;

	UFUNCTION()
	void OnRep_DamageTaken(const FGameplayAttributeData& OldDamageTaken) const;
};



