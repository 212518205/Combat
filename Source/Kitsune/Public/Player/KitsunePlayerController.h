// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KitsunePlayerControllerBase.h"
#include "GameFramework/PlayerController.h"
#include "KitsunePlayerController.generated.h"

class UInventoryItemInstance;
class UCommonActivatableWidget;
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
class KITSUNE_API AKitsunePlayerController : public AKitsunePlayerControllerBase
{
	GENERATED_BODY()

protected:
	virtual auto BeginPlay() -> void override;
	virtual void SetupInputComponent() override;
	virtual void OnControlledPawnChanged() override;
	virtual UKitsuneInputConfig* GetInputConfig() override;

	/***  UI接口   `BC@` ***/
	UFUNCTION(BlueprintCallable, Category = "UI")
	UCommonActivatableWidget* GetCurrentTopWidget() const;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	UInventoryItemInstance* GetSelectedInteractableItemInstance() const;

	/*** `@BC`   描述: 输入配置  `BC@` ***/

	/*** `@BC`   描述: 技能相关输入数据，需要在蓝图指定   `BC@` ***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UKitsuneInputConfig> InputConfig;

	/*** `@BC`   描述: InputAction 触发回调   `BC@` ***/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);

	void AbilityInputPressed(FGameplayTag TriggeredTag);
	void AbilityInputReleased(FGameplayTag TriggeredTag);
	void ToggleMouseMode(const FInputActionValue& InputActionValue);
	void OnLockOrSwitchTarget(const FInputActionValue& InputActionValue);
	
	/*** `@BC`   描述: 缓存所控制角色   `BC@` ***/
	UPROPERTY(Transient)
	UKitsuneAbilitySystemComponent* CachedKitsuneAbilitySystemComponent = nullptr;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UKitsuneAbilitySystemComponent* GetKitsuneASCFromPawn();

private:
	UPROPERTY(BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess))
	bool bForceMouse = false;
};
