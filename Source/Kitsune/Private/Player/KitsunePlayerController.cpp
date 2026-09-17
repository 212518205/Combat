// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KitsunePlayerController.h"

#include "CommonInputModeTypes.h"
#include "UIManagerSubsystem.h"
#include "AbilitySyetem/KitsuneAbilitySystemComponent.h"
#include"Characters/KitsuneCharacter.h"
#include "Component/Combat/PlayerCombatComponent.h"
#include "FunctionLibrary/KitsuneFunctionLibrary.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/KitsuneInputComponent.h"
#include "Inventory/InventorySystemComponent.h"
#include "UI/Widget/WidgetPrimaryLayout.h"
#include "UI/Widget/Game/WidgetMainHudScreen.h"


void AKitsunePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AKitsunePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UKitsuneInputComponent* KitsuneInputComponent = CastChecked<UKitsuneInputComponent>(InputComponent);
	if (!InputConfig) return;

	KitsuneInputComponent->BindAbilityInputAction(InputConfig, this,
	                                              &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased);

	KitsuneInputComponent->BindNativeInputAction(InputConfig, KitsuneGameplayTags::InputTag_Move,
	                                        ETriggerEvent::Triggered, this, &ThisClass::Move);
	KitsuneInputComponent->BindNativeInputAction(InputConfig, KitsuneGameplayTags::InputTag_Look,
	                                        ETriggerEvent::Triggered, this, &ThisClass::Look);
	KitsuneInputComponent->BindNativeInputAction(InputConfig, KitsuneGameplayTags::InputTag_Jump,
	                                        ETriggerEvent::Started, this, &ThisClass::Jump);
	KitsuneInputComponent->BindNativeInputAction(InputConfig, KitsuneGameplayTags::InputTag_ToggleMouseMode,
	                                        ETriggerEvent::Completed, this, &ThisClass::ToggleMouseMode);
	KitsuneInputComponent->BindNativeInputAction(InputConfig, KitsuneGameplayTags::InputTag_LockOrSwitchTarget,
	                                        ETriggerEvent::Completed, this, &ThisClass::OnLockOrSwitchTarget);
}

void AKitsunePlayerController::OnControlledPawnChanged()
{
	CachedKitsuneAbilitySystemComponent = nullptr;

	Super::OnControlledPawnChanged();
}

UKitsuneInputConfig* AKitsunePlayerController::GetInputConfig()
{
	return InputConfig;
}

UCommonActivatableWidget* AKitsunePlayerController::GetCurrentTopWidget() const
{
	return UUIManagerSubsystem::GetUIManager(GetWorld())->GetRegisteredPrimaryLayout()->GetTopWidget();
}

UInventoryItemInstance* AKitsunePlayerController::GetSelectedInteractableItemInstance() const
{
	if (const UWidgetMainHudScreen* MainHud = Cast<UWidgetMainHudScreen>(GetCurrentTopWidget()))
	{
		return MainHud->GetSelectedItemInstance();
	}
	return nullptr;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AKitsunePlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputAxisVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AKitsunePlayerController::Look(const FInputActionValue& Value)
{
	if (bForceMouse)return;

	const FVector2D InputAxisVector = Value.Get<FVector2D>();

	AddPitchInput(InputAxisVector.Y);
	AddYawInput(InputAxisVector.X);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AKitsunePlayerController::Jump(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ACharacter* ControlledCharacter = Cast<AKitsuneCharacter>(ControlledPawn))
		{
			ControlledCharacter->Jump();
		}
	}
}

void AKitsunePlayerController::AbilityInputPressed(const FGameplayTag TriggeredTag)
{
	if (UKitsuneAbilitySystemComponent* ASC = GetKitsuneASCFromPawn())
	{
		ASC->OnAbilityInputPressed(TriggeredTag);
	}
}

void AKitsunePlayerController::AbilityInputReleased(const FGameplayTag TriggeredTag)
{
}

void AKitsunePlayerController::ToggleMouseMode(const FInputActionValue& InputActionValue)
{
	// 通过 CommonUIActionRouter 获取当前激活的鼠标捕获模式
	if (UCommonUIActionRouterBase* Router = GetLocalPlayer()->GetSubsystem<UCommonUIActionRouterBase>())
	{
		// 获取当前真实的鼠标捕获模式
		const EMouseCaptureMode CurrentMode = Router->GetActiveMouseCaptureMode(EMouseCaptureMode::NoCapture);

		// 基于当前模式决定新的模式：当前为 NoCapture 则切换为永久捕获，否则切换回 NoCapture
		const EMouseCaptureMode NewCaptureMode = (CurrentMode == EMouseCaptureMode::NoCapture)
			                                         ? EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown
			                                         : EMouseCaptureMode::NoCapture;

		// 根据新模式决定是否在捕获时隐藏光标
		const bool bNewHideCursorDuringCapture = (NewCaptureMode != EMouseCaptureMode::NoCapture);

		// 构建新的输入配置
		const FUIInputConfig NewConfig(ECommonInputMode::All, NewCaptureMode, bNewHideCursorDuringCapture);

		// 应用新配置
		Router->SetActiveUIInputConfig(NewConfig, this);
	}
}

void AKitsunePlayerController::OnLockOrSwitchTarget(const FInputActionValue& InputActionValue)
{
	if (const APawn* OwningPawn = GetPawn())
	{
		if (UPlayerCombatComponent* Combat = OwningPawn->FindComponentByClass<UPlayerCombatComponent>())
		{
			Combat->SwitchLockedTarget();
		}
	}
}

UKitsuneAbilitySystemComponent* AKitsunePlayerController::GetKitsuneASCFromPawn()
{
	if (!CachedKitsuneAbilitySystemComponent)
	{
		if (const AKitsuneCharacter* KitsuneCharacter = GetPawn<AKitsuneCharacter>())
		{
			CachedKitsuneAbilitySystemComponent = Cast<UKitsuneAbilitySystemComponent>(
				KitsuneCharacter->GetAbilitySystemComponent());
		}
	}

	return CachedKitsuneAbilitySystemComponent;
}
