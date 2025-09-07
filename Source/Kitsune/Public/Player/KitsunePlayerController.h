// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KitsunePlayerController.generated.h"

struct FGameplayTag;
class UKitsuneInputConfig;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

USTRUCT(BlueprintType)
struct FActionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName ActionName;

	UPROPERTY(EditAnywhere)
	FKey ActionKey;
};

/**
 * 
 */
UCLASS()
class KITSUNE_API AKitsunePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Action Mapping")
	FKey GetActionKeyByAction(const FName& InActionName, const FKey& InActionKey = FKey(), const bool bLogNoFind = false);

protected:
	/** Function Begin*/
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	/** Function End*/

	/** Variable Begin*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> KitsuneContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputConfig")
	TObjectPtr<UKitsuneInputConfig> InputConfig;

	UPROPERTY(EditAnywhere, Category = "Action Mapping")
	TArray<FActionInfo> MappableAction;

	/** Variable End*/
private:
	/** Function Begin*/

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	/** Function End*/
};
