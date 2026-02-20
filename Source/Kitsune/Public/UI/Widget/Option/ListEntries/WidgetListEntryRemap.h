// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/Option/ListEntries/WidgetListEntryBase.h"
#include "Engine/Texture2D.h"
#include "WidgetListEntryRemap.generated.h"

class UInputKeySelector;
class UKitsuneCommonButtonBase;
class UListDataObjectRemap;
/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetListEntryRemap : public UWidgetListEntryBase
{
	GENERATED_BODY()

public:
	// 声明蓝图可重写事件，传递须显示键调用蓝图函数库显示Icon
	UFUNCTION(BlueprintImplementableEvent, Category = "ButtonStyle")
	void OnSetupButtonStyle(const FKey& InCurrentKey);
protected:
	virtual void OnOwningListDataHandle(UObject* ItemData) override;

	UFUNCTION()
	void OnMappableKeyChanged(FInputChord SelectedKey);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UKitsuneCommonButtonBase> KitsuneButton_ResetImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UInputKeySelector> InputKeySelector_MappableKey;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_ListEntryTitle;

private:
	UPROPERTY(Transient)
	TObjectPtr<UListDataObjectRemap> CachedDataObjectRemap;


};
