// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KitsunePlayerController.h"
#include"EnhancedInputSubsystems.h"
#include"EnhancedInputComponent.h"
#include "FrontendDebugHelper.h"
#include"Characters/KitsuneCharacter.h"
#include "Input/KitsuneInputComponent.h"
#include "UserSettings/EnhancedInputUserSettings.h"

FKey AKitsunePlayerController::GetActionKeyByAction(const FName& InActionName, const FKey& InActionKey, const bool bLogNoFind)
{
	for (auto& [ActionName,ActionKey]:MappableAction)
	{
		if (ActionName==InActionName)
		{
			ActionKey = InActionKey;
			return ActionKey;
		}
	}

	if (bLogNoFind)
	{
		Debug::Print(TEXT("No find Action, ActionName = ") + InActionName.ToString());
	}

	return FKey{};
}

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

	KitsuneInputComponent->BindAbilityActions(InputConfig, this,
		&ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AKitsunePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

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

void AKitsunePlayerController::Jump(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn<APawn>()) {
		if (ACharacter* ControlledCharacter = Cast<AKitsuneCharacter>(ControlledPawn)) {
			ControlledCharacter->Jump();
		}
	}
}

void AKitsunePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	Debug::Print(InputTag.ToString() + TEXT("Is Pressed"), 1,FColor::Red);
}

void AKitsunePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	Debug::Print(InputTag.ToString() + TEXT("Is Released"),3,FColor::Blue);
}

void AKitsunePlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	Debug::Print(InputTag.ToString() + TEXT("Is Held"),2,FColor::Yellow);
}
