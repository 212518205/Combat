// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KitsunePlayerController.h"
#include"EnhancedInputSubsystems.h"
#include"EnhancedInputComponent.h"
#include "FrontendDebugHelper.h"
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
		if (const auto InputUserSettings = Subsystem->GetUserSettings())
		{
			InputUserSettings->RegisterInputMappingContext(KitsuneContext);
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

}

void AKitsunePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

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

