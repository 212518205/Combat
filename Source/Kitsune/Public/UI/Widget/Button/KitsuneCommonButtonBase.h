// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "KitsuneCommonButtonBase.generated.h"

class UCommonTextBlock;

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	/** Function Begin*/
	UFUNCTION(BlueprintCallable)
	void SetButtonText(const FText InText)const;
	/** Function End*/

protected:
	/** Function Begin*/
	virtual void NativePreConstruct() override;
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	/** Function End*/

	/** Variable Begin*/
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", meta = (AllowPrivateAccess))
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", meta = (AllowPrivateAccess))
	FText ButtonDescriptionText;
	/** Variable End*/
};
