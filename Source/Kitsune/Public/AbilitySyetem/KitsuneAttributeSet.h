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
	UKitsuneAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**Public Variable Begin*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attribute");
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet,Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attribute");
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UKitsuneAttributeSet, MaxHealth);
	/**Public Variable End*/
protected:
	/** Protected Variable Begin*/
	/** Protected Variable End*/

	/**Protected Function Begin*/
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const  FGameplayEffectModCallbackData& Data) override;

	void SetEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& Props);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	/**Protected Function End*/
};



