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

	/**Public Variable Begin*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Main Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet,Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Main Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Main Attribute|Vitality")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Main Attribute|Vitality")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SpiritWisdom, Category = "Main Attribute|Magic")
	FGameplayAttributeData SpiritWisdom;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, SpiritWisdom)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxSpiritWisdom, Category = "Main Attribute|Magic")
	FGameplayAttributeData MaxSpiritWisdom;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, MaxSpiritWisdom)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Main Attribute|Physique")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, Vigor)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxVigor, Category = "Main Attribute|Physique")
	FGameplayAttributeData MaxVigor;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, MaxVigor)


	// 战斗属性（Combat Attribute）- 无Max值，战斗中生效的功能属性
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Toughness, Category = "Combat Attribute|Defense")
	FGameplayAttributeData Toughness;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, Toughness)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield, Category = "Combat Attribute|Defense")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, Shield)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MentalFocus, Category = "Combat Attribute|Defense")
	FGameplayAttributeData MentalFocus;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, MentalFocus)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Combat Attribute|Defense")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, Armor)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalStrikeChance, Category = "Combat Attribute|Offense")
	FGameplayAttributeData CriticalStrikeChance;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, CriticalStrikeChance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalStrikeMultiplier, Category = "Combat Attribute|Offense")
	FGameplayAttributeData CriticalStrikeMultiplier;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, CriticalStrikeMultiplier)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MentalCriticalChance, Category = "Combat Attribute|Offense")
	FGameplayAttributeData MentalCriticalChance;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, MentalCriticalChance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MentalCriticalMultiplier, Category = "Combat Attribute|Offense")
	FGameplayAttributeData MentalCriticalMultiplier;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, MentalCriticalMultiplier)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Combat Attribute|Penetration")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, ArmorPenetration)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MentalBurn, Category = "Combat Attribute|Penetration")
	FGameplayAttributeData MentalBurn;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, MentalBurn)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DodgeChance, Category = "Combat Attribute|Evasion")
	FGameplayAttributeData DodgeChance;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, DodgeChance)


	/**Public Variable End*/
protected:
	/** Protected Variable Begin*/
	/** Protected Variable End*/

	/**Protected Function Begin*/
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const  FGameplayEffectModCallbackData& Data) override;

	void SetEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& Props);

	// 气血系统
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	// 耐力系统
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;

	// 灵慧系统（江湖奇术消耗）
	UFUNCTION()
	void OnRep_SpiritWisdom(const FGameplayAttributeData& OldSpiritWisdom) const;

	UFUNCTION()
	void OnRep_MaxSpiritWisdom(const FGameplayAttributeData& OldMaxSpiritWisdom) const;

	// 体力系统（跳跃/闪避消耗）
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_MaxVigor(const FGameplayAttributeData& OldMaxVigor) const;

	// 韧性属性（降低僵直时间）
	UFUNCTION()
	void OnRep_Toughness(const FGameplayAttributeData& OldToughness) const;

	// 护盾属性（优先吸收伤害）
	UFUNCTION()
	void OnRep_Shield(const FGameplayAttributeData& OldShield) const;

	// 心神属性（抵御精神攻击）
	UFUNCTION()
	void OnRep_MentalFocus(const FGameplayAttributeData& OldMentalFocus) const;

	// 物理防御属性
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	// 暴击系统
	UFUNCTION()
	void OnRep_CriticalStrikeChance(const FGameplayAttributeData& OldCriticalStrikeChance) const;

	UFUNCTION()
	void OnRep_CriticalStrikeMultiplier(const FGameplayAttributeData& OldCriticalStrikeMultiplier) const;

	// 会心系统（精神暴击）
	UFUNCTION()
	void OnRep_MentalCriticalChance(const FGameplayAttributeData& OldMentalCriticalChance) const;

	UFUNCTION()
	void OnRep_MentalCriticalMultiplier(const FGameplayAttributeData& OldMentalCriticalMultiplier) const;

	// 穿透属性
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	// 心焰属性（抵扣心境）
	UFUNCTION()
	void OnRep_MentalBurn(const FGameplayAttributeData& OldMentalBurn) const;

	// 闪避属性
	UFUNCTION()
	void OnRep_DodgeChance(const FGameplayAttributeData& OldDodgeChance) const;
	/**Protected Function End*/
};



