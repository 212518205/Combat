// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetCharacterComponentObject.generated.h"

class UAttributeViewModel;
/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetCharacterComponentObject : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void OnBindAndInitialized(APawn* Pawn);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = OnBindAndInitialized))
	void BP_OnBindAndInitialized();
	
	UFUNCTION(BlueprintPure)
	bool IsLocalPlayerPawn() const;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APawn> OwningPawn;
	
	UFUNCTION(BlueprintCallable)
	UAttributeViewModel* GetOwningViewModel() const;
	
};
