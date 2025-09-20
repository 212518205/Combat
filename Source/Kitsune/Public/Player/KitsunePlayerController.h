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


/**
 * 
 */
UCLASS()
class KITSUNE_API AKitsunePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
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


	/** Variable End*/
private:
	/** Function Begin*/

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);

	/** Function End*/
};
