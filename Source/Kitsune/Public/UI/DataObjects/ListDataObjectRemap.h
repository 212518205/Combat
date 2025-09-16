// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DataObjects/ListDataObjectBase.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "ListDataObjectRemap.generated.h"

class UEnhancedPlayerMappableKeyProfile;
/**
 * 
 */
UCLASS()
class KITSUNE_API UListDataObjectRemap : public UListDataObjectBase
{
	GENERATED_BODY()
public:
	DATA_ACCESSOR(FKey, CurrentKey);
	DATA_ACCESSOR(FKey, DefaultKey);
	DATA_ACCESSOR(TObjectPtr<UEnhancedPlayerMappableKeyProfile>, Profiles);
	DATA_ACCESSOR(TObjectPtr<UEnhancedInputUserSettings>, InputUserSettings);
	DATA_ACCESSOR(EPlayerMappableKeySlot, CurrentKeySlot);
	bool IsGamepadKey() const;
	void UserMappableKeyChanged(const FKey& InNewKey, EOptionsListDataModifyReason Reason=EOptionsListDataModifyReason::DirectlyModified);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCurrentKeyChangedSignature,const FKey&, CurrentKey);
	FOnCurrentKeyChangedSignature OnCurrentKeyChangedDelegate;
private:
	FKey CurrentKey;
	FKey DefaultKey;
	UPROPERTY(Transient)
	TObjectPtr<UEnhancedPlayerMappableKeyProfile> Profiles;

	UPROPERTY(Transient)
	TObjectPtr<UEnhancedInputUserSettings> InputUserSettings;

	EPlayerMappableKeySlot CurrentKeySlot;

	FPlayerKeyMapping* GetPlayerKeyMapping() const;
};
