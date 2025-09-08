// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OptionsDataRegistry.generated.h"

class UListDataObjectBase;
class UListDataObjectCollection;
/**
 * 
 */
UCLASS()
class KITSUNE_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()
	
public:
	/** Function Begin ***/
	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);
	const TArray<UListDataObjectCollection*>& GetRegisteredOptionsTabCollections() const { return RegisteredOptionsTabCollections; }
	TArray<UListDataObjectBase*> GetAllListDataByTabID(const FName& InTabID);
	/** Function End ***/

private:
	/** Function Begin*/
	void InitGameplayTab();
	void InitAudioTab();
	void InitVideoTab();
	void InitControlTab();

	static void FindChildListDataRecursively(UListDataObjectBase* InParentData, TArray<UListDataObjectBase*>& OutFoundChildListData);
	/** Function End*/

	/** Variable Begin*/
	UPROPERTY(Transient)
	TArray<UListDataObjectCollection*> RegisteredOptionsTabCollections;
	/** Variable End*/
};
