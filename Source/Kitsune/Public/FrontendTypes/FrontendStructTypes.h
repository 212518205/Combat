#pragma once
#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "FrontendStructTypes.generated.h"



USTRUCT(BlueprintType)
struct FConfirmScreenButtonInfo
{
	GENERATED_BODY()

public:
	FConfirmScreenButtonInfo(){}
	FConfirmScreenButtonInfo(const FText& InButtonTextDisplay,const EConfirmScreenButtonResult InButtonResult):
		ButtonTextDisplay(InButtonTextDisplay),ButtonResult(InButtonResult){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonTextDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonResult ButtonResult = EConfirmScreenButtonResult::Unknown;
};


USTRUCT(BlueprintType)
struct FMenuTileRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu Tile")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu Tile")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu Tile")
	bool bIsShowAsNew;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu Tile")
	FText HotKeyText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu Tile")
	int32 RequiredLevel = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu Tile")
	ETileViewEntryType EntryType;

};


