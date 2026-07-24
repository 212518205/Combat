// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "KitsuneActivatableWidgetStack.generated.h"

class UWidgetActivatableBase;
/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneActivatableWidgetStack : public UWidget
{
	GENERATED_BODY()
	
public:
	UKitsuneActivatableWidgetStack(const FObjectInitializer& Initializer);
	
	UFUNCTION(BlueprintCallable)
	UWidgetActivatableBase* PushWidget(UPARAM(meta = (Categories = "UI.Widget")) const FGameplayTag InWidgetTag, UClass* WidgetClass);	
	
	UFUNCTION(BlueprintCallable)
	void PopWidget();
	
	UFUNCTION(BlueprintCallable)
	UWidgetActivatableBase* GetTopWidget();
	
	UFUNCTION(BlueprintCallable)
	UWidgetActivatableBase* GetActivatableWidgetByTag(UPARAM(meta = (Categories = "UI.Widget")) const FGameplayTag InWidgetTag) const;
	
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
private:
	TSharedPtr<SCommonAnimatedSwitcher> MySwitcher;
	
	UPROPERTY(Transient)
	TArray<UWidgetActivatableBase*> StackWidgets;
	
	UPROPERTY(Transient)
	TMap<FGameplayTag, UWidgetActivatableBase*> CachedWidgets;
	
	UPROPERTY(Transient)
	UWidgetActivatableBase* CurrentTopWidget = nullptr;
	
	void HandleActiveIndexChanged(int32 NewIndex);
	void HandleTransitioningChanged(bool bIsTransitioning);
	
};
