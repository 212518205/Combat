// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DataObjects/ListDataObjectBase.h"
#include "ListDataObjectCollection.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UListDataObjectCollection : public UListDataObjectBase
{
	GENERATED_BODY()

public:
	/** Function Begin*/
	void AddChildListData(UListDataObjectBase* InChildListData);

	virtual TArray<UListDataObjectBase*> GetChildListData() const override;
	virtual bool HasChildListData() const override;
	/** Function End*/

private:
	/** Variable Begin*/
	UPROPERTY(Transient)
	TArray<UListDataObjectBase*> ChildListData;
	/** Variable End*/
};
