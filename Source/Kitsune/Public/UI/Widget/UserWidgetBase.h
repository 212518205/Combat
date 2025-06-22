// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetBase.generated.h"

class UCharacterStateViewModel;
/**
 * 
 */
UCLASS()
class KITSUNE_API UUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Category="ViewModel")
	virtual void SetCharacterStateViewModel(UCharacterStateViewModel* CS_VM);
protected:
	UFUNCTION(BlueprintImplementableEvent,Category="ViewModel|Event")
	void CharacterStateViewModelSet();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="ViewModel")
	TObjectPtr<UCharacterStateViewModel> CSViewModel;

	UFUNCTION(BlueprintCallable,Category="ViewModel")
	FORCEINLINE UCharacterStateViewModel* GetCharacterStateViewModel() { return CSViewModel; }
};
