// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// ===== [修改] 由 UKitsuneClickableWidget 改为 UCommonButtonBase，统一到 CommonUI 的可点击体系 =====
#include "CommonButtonBase.h"
#include "Components/Image.h"
#include "MenuEntryBase.generated.h"

class UMenuEntryData;
class UCommonLazyImage;
class UCommonTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class KITSUNE_API UMenuEntryBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	/***   ...IUserObjectListEntry Interface Begin...   ***/
	void NativeOnEntryInitialize(UObject* ListItemObject);
	/***   ...IUserObjectListEntry Interface End...     ***/
	 
protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Image_DisplayImage;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock>  CommonTextBlock_DisplayName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock>  CommonTextBlock_ShowAsNewText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_HotKeyText;

	UPROPERTY(BlueprintReadOnly)
	UMenuEntryData* CachedEntryData;

	/***   ...UCommonButtonBase Interface Begin...   ***/
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	virtual void NativeOnClicked() override;
	/***   ...UCommonButtonBase Interface End...     ***/

	/*** `@BC`   描述: 蓝图配置   `BC@` ***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	FLinearColor DefaultIconColor = FLinearColor(0.f, 0.f, 0.f, 1.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	FLinearColor HoverIconColor = FLinearColor(255.f, 248.f, 220.f, 1.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	FLinearColor ClickIconColor = FLinearColor(255.f, 215.f, 0.f, 1.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	float DefaultBackgroundOpacity = 0.6;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void RevertWidgetStyle() const { Image_DisplayImage->SetColorAndOpacity(DefaultIconColor); }

	UFUNCTION(BlueprintCallable)
	static void SetImageOpacity(UImage* InModifiedImage, const float InOpacity) { InModifiedImage->SetOpacity(InOpacity); }
};
