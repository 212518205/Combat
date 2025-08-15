// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "KitsuneCharacter.generated.h"



/**
 * 
 */
UCLASS(Abstract)
class KITSUNE_API AKitsuneCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AKitsuneCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
protected:
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UStaticMeshComponent> Sword;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UStaticMeshComponent> Sheath;
private:
	virtual void InitAbilityInfo() override;
};
