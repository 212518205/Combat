// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KitsunePlayerController.h"

#include "CommonInputModeTypes.h"
#include"EnhancedInputSubsystems.h"
#include"EnhancedInputComponent.h"
#include "UIManagerSubsystem.h"
#include "AbilitySyetem/KitsuneAbilitySystemComponent.h"
#include"Characters/KitsuneCharacter.h"
#include "FunctionLibrary/KitsuneFunctionLibrary.h"
#include "Game/KitsunePlayerState.h"
#include "Game/GameInstanceSubsystem/KitsuneSaveSubsystem.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/KitsuneInputComponent.h"
#include "Inventory/InventorySystemComponent.h"
#include "UI/Widget/WidgetPrimaryLayout.h"
#include "UI/Widget/Game/WidgetMainHudScreen.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void AKitsunePlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(IMC_GAS_Skills);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) {
		Subsystem->AddMappingContext(IMC_GAS_Skills, 0);
		Subsystem->AddMappingContext(IMC_Gameplay, 50);
		if (const auto InputUserSettings = Subsystem->GetUserSettings())
		{
			InputUserSettings->RegisterInputMappingContext(IMC_GAS_Skills);
			InputUserSettings->RegisterInputMappingContext(IMC_Gameplay);
		}
	}
	
	if (UKitsuneSaveSubsystem* SaveSubsystem = UKitsuneSaveSubsystem::GetSaveSubsystem(this); SaveSubsystem && IsLocalController())
	{
		const FString LocalCredential = SaveSubsystem->GetOrCreateLocalCredential();
		if (HasAuthority())
		{
			OnCredentialReported(LocalCredential);
		}
		else
		{
			Server_ReportLocalCredential(LocalCredential);
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

	KitsuneInputComponent->BindAction(ShowOrHiddenMouseAction, ETriggerEvent::Completed, this, &ThisClass::ToggleMouseMode);

	KitsuneInputComponent->BindAbilityInputAction(AbilityInputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased);
}

void AKitsunePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void AKitsunePlayerController::Server_ReportLocalCredential_Implementation(const FString& InCredential)
{
	OnCredentialReported(InCredential);
}

void AKitsunePlayerController::OnCredentialReported(const FString& InCredential) const
{
	AKitsunePlayerState* KitsunePlayerState = GetPlayerState<AKitsunePlayerState>();
	if (!KitsunePlayerState)return;
	if (const int64 PlayerUID = UKitsuneSaveSubsystem::ResolvePlayerCredential(InCredential); PlayerUID >= 0)
	{
		 KitsunePlayerState->SetPlayerUID(PlayerUID);
		if (const AKitsuneCharacter* KitsuneCharacter = Cast<AKitsuneCharacter>(GetPawn()))
		{
			KitsuneCharacter->BindAndLoadSave();
		}
	}
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
void AKitsunePlayerController::ToggleMouseMode(const FInputActionValue& InputActionValue)
{
	// 通过 CommonUIActionRouter 获取当前激活的鼠标捕获模式
	if (UCommonUIActionRouterBase* Router = GetLocalPlayer()->GetSubsystem<UCommonUIActionRouterBase>())
	{
		// 获取当前真实的鼠标捕获模式
		EMouseCaptureMode CurrentMode = Router->GetActiveMouseCaptureMode(EMouseCaptureMode::NoCapture);
        
		// 基于当前模式决定新的模式：当前为 NoCapture 则切换为永久捕获，否则切换回 NoCapture
		EMouseCaptureMode NewCaptureMode = (CurrentMode == EMouseCaptureMode::NoCapture)
			? EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown
			: EMouseCaptureMode::NoCapture;
        
		// 根据新模式决定是否在捕获时隐藏光标
		bool bNewHideCursorDuringCapture = (NewCaptureMode != EMouseCaptureMode::NoCapture);
        
		// 构建新的输入配置
		FUIInputConfig NewConfig(ECommonInputMode::All, NewCaptureMode, bNewHideCursorDuringCapture);
        
		// 应用新配置
		Router->SetActiveUIInputConfig(NewConfig, this);
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

