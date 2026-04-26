// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "KitsuneCommonButtonBase.generated.h"

class UCommonTextBlock;
class UCommonLazyImage;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class KITSUNE_API UKitsuneCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	/** Function Begin*/
	UFUNCTION(BlueprintCallable)
	void SetButtonText(const FText InText)const;

	UFUNCTION(BlueprintCallable)
	virtual void SetButtonStyle(const FSlateBrush& InSlateBrush);
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

	/***  按钮图标   `BC@` ***/
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonLazyImage> CommonLazyImage_ButtonImage;

	/***  按钮背景图片   `BC@` ***/
	UPROPERTY(EditDefaultsOnly)
	bool bBackground;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UCommonLazyImage> CommonLazyImage_Background;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bBackground"))
	TSoftObjectPtr<UTexture2D> T_HoveredBackground;

	/***  按钮文本信息   `BC@` ***/
	UPROPERTY(EditDefaultsOnly)
	bool bTextStyle;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bTextStyle"))
	TSubclassOf<UCommonTextStyle> HoveredTextStyle;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bTextStyle"))
	TSubclassOf<UCommonTextStyle> UnHoveredTextStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", meta = (AllowPrivateAccess))
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", meta = (AllowPrivateAccess))
	FText ButtonDescriptionText;
	/** Variable End*/
};
