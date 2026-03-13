// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/WidgetActivatableBase.h"
#include "WidgetLocalMenu.generated.h"

class UPlayerViewModel;
class UTileViewEntryData;
class UKitsuneCommonTileView;
/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetLocalMenu : public UWidgetActivatableBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	UPlayerViewModel* GetLocalPlayerViewModel();

	/*** `@BC`   描述: 返回true代表   `BC@` ***/
	void LoadAllMenuTileData();
	void RefreshMenu();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> MenuDataTable;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UKitsuneCommonTileView> TileView_MenuTile;

	UPROPERTY(BlueprintReadOnly)
	TArray<UTileViewEntryData*> CachedMenuTileData;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPlayerViewModel> CachedLocalViewModel;
};
