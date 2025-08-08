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

