// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
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
class KITSUNE_API AKitsunePlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AKitsunePlayerController();

	/***   ...IGenericTeamAgentInterface Interface Begin...   ***/
	virtual FGenericTeamId GetGenericTeamId() const override;
	/***   ...IGenericTeamAgentInterface Interface End...     ***/

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
	TObjectPtr<UInputMappingContext> UIInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> ShowOrHiddenMouseAction;

	/*** `@BC`   描述: 技能相关输入数据，需要在蓝图指定   `BC@` ***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UKitsuneInputConfig> AbilityInputConfig;

	/*** `@BC`   描述: InputAction 触发回调   `BC@` ***/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);

	void AbilityInputPressed(FGameplayTag TriggeredTag);
	void AbilityInputReleased(FGameplayTag TriggeredTag);

	void OnPressed_ShowMouse(const FInputActionValue& Value);
	void OnReleased_ShowMouse(const FInputActionValue& Value);
	void UpdateMouse();
	/*** `@BC`   描述: 缓存所控制角色   `BC@` ***/
	UPROPERTY(Transient)
	UKitsuneAbilitySystemComponent* CachedKitsuneAbilitySystemComponent = nullptr;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UKitsuneAbilitySystemComponent* GetKitsuneASCFromPawn();

private:
	FGenericTeamId PlayerTeamId;

	UPROPERTY(BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess))
	bool bForceMouse = false;
};
