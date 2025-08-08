#pragma once
#include "CoreMinimal.h"
#include "FrontendEnumTypes.generated.h"

UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	Ok,
	YesNo,
	OkCancel,
	Unknown UMETA(Hidden)
};
 
UENUM(BlueprintType)
enum class EConfirmScreenButtonResult : uint8
{
	Confirmed,
	Cancelled,
	Unknown UMETA(Hidden)
};



