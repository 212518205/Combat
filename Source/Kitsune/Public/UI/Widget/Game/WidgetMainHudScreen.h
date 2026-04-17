// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/PlayerViewModel.h"
#include "UI/Widget/WidgetActivatableBase.h"
#include "WidgetMainHudScreen.generated.h"

class UUIManagerSubsystem;
class UCommonTextBlock;
class UKitsuneCommonListView;
class UInventoryItemInstance;
class UPlayerViewModel;
/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetMainHudScreen : public UWidgetActivatableBase
{
	GENERATED_BODY()
	
public:
	UInventoryItemInstance* GetSelectedItemInstance() const;

protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void OnInteractableItemChange(UInventoryItemInstance* ItemInstance, EItemInstanceAction InstanceAction);

	UFUNCTION(BlueprintCallable, Category = "Initialize")
	void InitializeMainHudScreen();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UKitsuneCommonListView> CommonListView_Prompt;

	UPROPERTY(BlueprintReadOnly, Category = "Cached")
	TObjectPtr<UPlayerViewModel> CachedPlayerViewModel;
	
	UPROPERTY(BlueprintReadOnly, Category = "Cached")
	TObjectPtr<UUIManagerSubsystem> CachedUIManager;
	
private:
	void ChangeSelectionByOffset(int32 Offset) const;

};
