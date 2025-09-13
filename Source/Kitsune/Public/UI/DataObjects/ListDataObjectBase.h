// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "UObject/NoExportTypes.h"
#include "ListDataObjectBase.generated.h"

#define DATA_ACCESSOR(DataType,PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName; } \
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName;}

/**
 * 
 */
UCLASS(Abstract)
class KITSUNE_API UListDataObjectBase : public UObject
{
	GENERATED_BODY()

public:
	/** Function Begin*/
	DATA_ACCESSOR(FName, DataID);
	DATA_ACCESSOR(FText, DataDisplayName);
	DATA_ACCESSOR(FText, DescriptionRichText);
	DATA_ACCESSOR(FText, DisabledRichText);
	DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage);
	DATA_ACCESSOR(UListDataObjectBase*, ParentData);

	virtual TArray<UListDataObjectBase*> GetChildListData() const {
		return {};
	}

	virtual bool HasChildListData() const { return false; }

	void InitDataObject();
	/** Function End*/
	/** Variable Begin*/
	/** Variable End*/

protected:
	/** Function Begin*/
	virtual void OnDataInitialized();
	/** Function End*/

private:
	/** Variable Begin*/
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;

	UPROPERTY(Transient)
	UListDataObjectBase* ParentData;
	/** Variable End*/
	
};
