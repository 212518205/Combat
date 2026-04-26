// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KitsunePlayerController.h"

#include "CommonInputModeTypes.h"
#include"EnhancedInputSubsystems.h"
#include"EnhancedInputComponent.h"
#include "FrontendDebugHelper.h"
#include "UIManagerSubsystem.h"
#include "AbilitySyetem/KitsuneAbilitySystemComponent.h"
#include"Characters/KitsuneCharacter.h"
#include "Component/Interaction/InteractionComponent.h"
#include "FunctionLibrary/KitsuneFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "Input/KitsuneInputComponent.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventorySystem.h"
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
}

void AKitsunePlayerController::OnInteraction(const FInputActionValue& InputActionValue)
{
	if (UKitsuneFunctionLibrary::NativeDoesActorHaveTag(GetPawn(), KitsuneGameplayTags::Player_Status_Pickupable))
	{
		if (IPawnInteractInterface* Interact = Cast<IPawnInteractInterface>(GetPawn()))
		{
			if (UInventorySystem* InventorySystem = Interact->GetInteractionComp()->GetInventorySystem())
			{
				if (UInventoryItemInstance* ItemInstance = GetSelectedInteractableItemInstance())
				{
					InventorySystem->AddItem(GetSelectedInteractableItemInstance());
					GetSelectedInteractableItemInstance()->GetOwningActor()->Destroy();
				}
			}
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
	KitsuneInputComponent->BindAction(PickupableAction, ETriggerEvent::Completed, this, &ThisClass::OnInteraction);

	KitsuneInputComponent->BindAbilityInputAction(AbilityInputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased);
}

void AKitsunePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

UCommonActivatableWidget* AKitsunePlayerController::GetCurrentTopWidget() const
{
	return UUIManagerSubsystem::GetUIManager(GetWorld())->GetRegisteredPrimaryLayout()->GetTopWidget();
}

UInventoryItemInstance* AKitsunePlayerController::GetSelectedInteractableItemInstance() const
{
	if (UWidgetMainHudScreen* MainHud = Cast<UWidgetMainHudScreen>(GetCurrentTopWidget()))
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

void AKitsunePlayerController::PrintInventory()
{ // 获取本地控制的 Pawn
	APawn* MyPawn = GetPawn();
	if (!MyPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("无法获取本地 Pawn"));
		return;
	}

	// 假设你的 InventorySystem 是 Pawn 身上的一个组件，名为 "InventoryComponent"
	UInventorySystem* InventorySys = Cast<AKitsuneCharacter>(MyPawn)->GetInteractionComp()->GetInventorySystem();
    
	if (!InventorySys)
	{
		// 如果 InventorySystem 不是组件而是其他方式持有，请替换为你的获取逻辑
		UE_LOG(LogTemp, Warning, TEXT("无法在 Pawn 上找到 InventorySystem"));
		return;
	}

	InventorySys->DebugPrintInventory();
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
			EMouseCaptureMode::NoCapture,
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

