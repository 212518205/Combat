// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "KitsuneClickableWidget.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;
class UImage;
/**
 *
 */
UCLASS()
class KITSUNE_API UKitsuneClickableWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnMouseEventDelegate);

	FOnMouseEventDelegate OnMouseEnter;
	FOnMouseEventDelegate OnMouseLeave;
	FOnMouseEventDelegate OnMouseDown;
	FOnMouseEventDelegate OnMouseUp;


protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	bool bIsHovered = false;
	bool bIsClicked = false;

	/*** `@BC`   描述: C++子类重写 调用BlueprintImplementableEvent  `BC@` ***/
	virtual void MouseEnter();
	virtual void MouseLeave();
	virtual void MouseDown();
	virtual void MouseUp();

	/*** `@BC`   描述: 蓝图扩展逻辑   `BC@` ***/
	UFUNCTION(BlueprintImplementableEvent, Category = "MouseEvent")
	void MouseEnterEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "MouseEvent")
	void MouseLeaveEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "MouseEvent")
	void MouseDownEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "MouseEvent")
	void MouseUpEvent();

};