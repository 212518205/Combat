// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "WidgetAbilityList.generated.h"

class UPlayerViewModel;
class UWidgetAbilityEntry;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetAbilityList : public UUserWidget
{
	GENERATED_BODY()
	
public:	
	
protected:
	UWidget* GetEntryByIndex(const int32 Index) const;
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Up;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Down;
	
	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TSubclassOf<UWidgetAbilityEntry> AbilityEntryClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 UpRowSlotCount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DownRowSlotCount;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FAbilityUIData> AbilityUIDates;
	
	UFUNCTION()
	void HandleAbilityChanged(FGameplayAbilitySpecHandle& SpecHandle);
	
	void RefreshAbilitySlots();
	
private:
	UPlayerViewModel* GetLocalPlayerViewModel();
	
	UPROPERTY()
	UPlayerViewModel* CachedLocalViewModel;
	
};
