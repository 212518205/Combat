// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "KitsuneHUD.generated.h"

struct FModelSet;
class UCharacterStateViewModel;
class UUserWidgetOverlay;
class UAttributeSet;
class UAbilitySystemComponent;
class UUserWidgetBase;
/**
 * 
 */
UCLASS()
class KITSUNE_API AKitsuneHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitCharacterState(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable,Category="ViewModel")
	UCharacterStateViewModel* GetCSViewModel(const FModelSet& ModelSet);

	UPROPERTY(EditAnywhere,Category="CharStateWidget")
	TObjectPtr<UUserWidgetOverlay> CharStateWidget;

	UPROPERTY(EditAnywhere,Category="ViewModel")
	TObjectPtr<UCharacterStateViewModel> CSViewModel;

	UPROPERTY(EditAnywhere,Category="CharStateWidget")
	TSubclassOf<UUserWidgetOverlay> OverlayWidgetClass;

	UPROPERTY(EditAnywhere,Category="ViewModel")
	TSubclassOf<UCharacterStateViewModel> CSViewModelClass;


};
