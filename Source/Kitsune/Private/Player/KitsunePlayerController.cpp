// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KitsunePlayerController.h"

#include "CommonInputModeTypes.h"
#include"EnhancedInputSubsystems.h"
#include"EnhancedInputComponent.h"
#include "FrontendDebugHelper.h"
#include "UIManagerSubsystem.h"
#include "AbilitySyetem/KitsuneAbilitySystemComponent.h"
#include"Characters/KitsuneCharacter.h"
#include "Input/KitsuneInputComponent.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void AKitsunePlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(KitsuneContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) {
		Subsystem->AddMappingContext(KitsuneContext, 0);
		Subsystem->AddMappingContext(UIInputMappingContext, 50);
		if (const auto InputUserSettings = Subsystem->GetUserSettings())
		{
			InputUserSettings->RegisterInputMappingContext(KitsuneContext);
			InputUserSettings->RegisterInputMappingContext(UIInputMappingContext);
		}
	}
}

void AKitsunePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UKitsuneInputComponent* KitsuneInputComponent = CastChecked<UKitsuneInputComponent>(InputComponent);
	KitsuneInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, 
		this,&AKitsunePlayerController::Move);
	KitsuneInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, 
		this,&AKitsunePlayerController::Look);
	KitsuneInputComponent->BindAction(JumpAction, ETriggerEvent::Started, 
		this,&AKitsunePlayerController::Jump);

	KitsuneInputComponent->BindAction(ShowOrHiddenMouseAction, ETriggerEvent::Started, this, &ThisClass::OnPressed_ShowMouse);
	KitsuneInputComponent->BindAction(ShowOrHiddenMouseAction, ETriggerEvent::Completed, this, &ThisClass::OnReleased_ShowMouse);

	KitsuneInputComponent->BindAbilityInputAction(AbilityInputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased);
}

void AKitsunePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

AKitsunePlayerController::AKitsunePlayerController()
{
	PlayerTeamId = FGenericTeamId(0);
}

FGenericTeamId AKitsunePlayerController::GetGenericTeamId() const
{
	return PlayerTeamId;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AKitsunePlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputAxisVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if (APawn* ControlledPawn = GetPawn<APawn>()) {
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
	if (APawn* ControlledPawn = GetPawn<APawn>()) {
		if (ACharacter* ControlledCharacter = Cast<AKitsuneCharacter>(ControlledPawn)) {
			ControlledCharacter->Jump();
		}
	}
}

void AKitsunePlayerController::AbilityInputPressed(const FGameplayTag TriggeredTag)
{
	GetKitsuneASCFromPawn()->OnAbilityInputPressed(TriggeredTag);
}

void AKitsunePlayerController::AbilityInputReleased(const FGameplayTag TriggeredTag)
{
}

void AKitsunePlayerController::OnPressed_ShowMouse(const FInputActionValue& Value)
{
	bForceMouse = true;
	UpdateMouse();
}

void AKitsunePlayerController::OnReleased_ShowMouse(const FInputActionValue& Value)
{
	bForceMouse = false;
	UpdateMouse();
}

void AKitsunePlayerController::UpdateMouse()
{
	if (bForceMouse)
	{
		UUIManagerSubsystem::ExistedSetInputMode(
			this,
			ECommonInputMode::All,                        // 或 ECommonInputMode::GameAndMenu
			EMouseCaptureMode::CaptureDuringMouseDown,
			false   
		);
	}
	else
	{
		UUIManagerSubsystem::ExistedSetInputMode(
			this,
			ECommonInputMode::Game,
			EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown,
			true   
		);
	}
}

UKitsuneAbilitySystemComponent* AKitsunePlayerController::GetKitsuneASCFromPawn()
{
	if (!CachedKitsuneAbilitySystemComponent)
	{
		if (const AKitsuneCharacter* KitsuneCharacter = GetPawn<AKitsuneCharacter>()) {
			CachedKitsuneAbilitySystemComponent = CastChecked<UKitsuneAbilitySystemComponent>(
				KitsuneCharacter->GetAbilitySystemComponent());
		}
	}

	return CachedKitsuneAbilitySystemComponent;
}

