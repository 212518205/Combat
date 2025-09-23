﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KitsunePlayerController.generated.h"

class UKitsuneAbilitySystemComponent;
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
	/***   ...APlayerController Interface Begin...   ***/
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	/***   ...APlayerController Interface End...     ***/

	/*** `@BC`   描述: 输入上下文以及输入动作   `BC@` ***/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> KitsuneContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	/*** `@BC`   描述: 技能相关输入数据，需要在蓝图指定   `BC@` ***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UKitsuneInputConfig> AbilityInputConfig;

	/*** `@BC`   描述: InputAction 触发回调   `BC@` ***/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);

	void AbilityInputPressed(FGameplayTag TriggeredTag);
	void AbilityInputReleased(FGameplayTag TriggeredTag);

	/*** `@BC`   描述: 缓存所控制角色，减少消耗   `BC@` ***/
	UPROPERTY(Transient)
	UKitsuneAbilitySystemComponent* CachedKitsuneAbilitySystemComponent;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UKitsuneAbilitySystemComponent* GetKitsuneASCFromPawn();
};
