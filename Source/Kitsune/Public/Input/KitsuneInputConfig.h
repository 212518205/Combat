// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "KitsuneInputConfig.generated.h"

/**
 * 
 */

class UInputMappingContext;
class UInputAction;

USTRUCT(BlueprintType)
struct FKitsuneInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly,meta = (Categories = "InputTag"))
	FGameplayTag InputTag = FGameplayTag();

};

USTRUCT()
struct FKitsuneInputMapping
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> IMC;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Priority = -1;
	
	UPROPERTY(EditDefaultsOnly)
	bool bRegisterWithSettings = false;
	
	bool IsValid() const { return IMC != nullptr && Priority != -1; }
};

UCLASS()
class KITSUNE_API UKitsuneInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:	
	const UInputAction* FindNativeActionByTag(const FGameplayTag& InTag, const bool bNoFindLog = false);
	const UInputAction* FindSkillsInputActionByTag(const FGameplayTag& InTag, const bool bNoFindLog = false);
	TArray<FKitsuneInputMapping> GetInputMappings() const { return InputMappings; }
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FKitsuneInputAction> SkillsInputActions;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FKitsuneInputAction> NativeInputActions;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FKitsuneInputMapping> InputMappings;
	
};
