// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ListenCooldownChange.generated.h"

class UKitsuneAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCooldownStart, float, RemainingTime, float, Duration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownEnd, float, Duration);


/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class KITSUNE_API UListenCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", HidePin="WorldContextObject", WorldContext = "WorldContextObject"))
	static UListenCooldownChange* ListenCooldownChange(
		UObject* WorldContextObject, 
		UKitsuneAbilitySystemComponent* ASC,
		UPARAM(meta = (Categories = "UI.Widget")) FGameplayTag InCooldownTag,
		FGameplayAbilitySpecHandle Handle);
	
	virtual void Activate() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnCooldownStart CooldownStart;
	
	UPROPERTY(BlueprintAssignable)
	FOnCooldownEnd CooldownEnd;
	
protected:
	UFUNCTION(BlueprintCallable)
	void EndTask();
	
private:
	TWeakObjectPtr<UKitsuneAbilitySystemComponent> CachedASC;
	FGameplayTag CachedCooldownTag;
	FGameplayAbilitySpecHandle CachedAbilitySpecHandle;
	
	UFUNCTION()
	void OnCooldownTagChanged(const FGameplayTag Tag, int32 Count);
	
	FDelegateHandle TagChangeDelegateHandle;
	
};
