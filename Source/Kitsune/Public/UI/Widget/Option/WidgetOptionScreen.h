// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/WidgetActivatableBase.h"
#include "WidgetOptionScreen.generated.h"

class UKitsuneCommonListView;
class UKitsuneTabListWidgetBase;
class UOptionsDataRegistry;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class KITSUNE_API UWidgetOptionScreen : public UWidgetActivatableBase
{
	GENERATED_BODY()

protected:
	/** Function Begin*/
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;

	UFUNCTION()
	void OnTabSelected(FName TabId);
	/** Function End*/

private:
	/** Function Begin*/
	UOptionsDataRegistry* GetDataRegistry();
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();
	/** Function End*/

	/** Variable Begin*/
	UPROPERTY(meta = (BindWidget))
	UKitsuneTabListWidgetBase* TabListWidget_OptionsTabs;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget,AllowPrivateAccess="true"))
	UKitsuneCommonListView* CommonListView_OptionsList;

	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;

	UPROPERTY(Transient)
	UOptionsDataRegistry* OwningDataRegistry;
	/** Variable End*/
};
