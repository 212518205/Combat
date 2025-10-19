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

UENUM(BlueprintType)
enum class EOptionsListDataModifyReason : uint8
{
	DirectlyModified,
	DependencyModified,
	ResetToDefault
};

UENUM()
enum class EKitsuneValidType: uint8
{
	Valid,
	InValid
};

UENUM()
enum class EKitsuneSuccessType: uint8
{
	Failed,
	Successful
};


