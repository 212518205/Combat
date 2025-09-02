// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"AbilitySystemInterface.h"
#include "CharacterBase.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class KITSUNE_API ACharacterBase : public ACharacter , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;
protected:
	/** Function Begin*/
	virtual void BeginPlay() override;

	virtual void InitAbilityInfo();

	void InitializeAttributes() const;
	/** Function End*/

	/** Variable Begin*/
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Attribute")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	/** Variable End*/
};
