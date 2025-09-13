// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "KitsuneGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	static UKitsuneGameUserSettings* GetGameUserSettings();

	UFUNCTION()
	FString GetCurrentGameDifficulty();

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InGameDifficulty);

	/***  用于测试   ****/
	// 自动瞄准设置
	UFUNCTION()
	FString GetCurrentAutoAim() { return CurrentAutoAim; }

	UFUNCTION()
	void SetCurrentAutoAim(const FString& InAutoAim) { CurrentAutoAim = InAutoAim; }
	// 自动存档设置
	UFUNCTION()
	FString GetCurrentAutoSave() { return CurrentAutoSave; }

	UFUNCTION()
	void SetCurrentAutoSave(const FString& InAutoSave) { CurrentAutoSave = InAutoSave; }

private:
	UPROPERTY(Config)
	FString CurrentGameDifficulty;

	// 自动瞄准配置变量（保存当前选择的自动瞄准强度）
	UPROPERTY(Config)
	FString CurrentAutoAim;

	// 自动存档配置变量（保存当前选择的自动存档频率）
	UPROPERTY(Config)
	FString CurrentAutoSave;
	
};
