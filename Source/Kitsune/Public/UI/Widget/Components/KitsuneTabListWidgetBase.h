// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "KitsuneTabListWidgetBase.generated.h"

class UKitsuneCommonButtonBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class KITSUNE_API UKitsuneTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:
	void RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName);

private:
	/** Function Begin*/
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	/** Function End*/
	/** Variable Begin*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "List Widget Settings", meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "10"))
	int32 DebugEditorPreviewTabCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "List Widget Settings", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UKitsuneCommonButtonBase> TabButtonEntryWidgetClass;

	/** Variable End*/
};
