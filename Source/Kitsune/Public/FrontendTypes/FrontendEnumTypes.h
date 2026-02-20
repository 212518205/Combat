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


UENUM(BlueprintType, meta = (Bitflags, UseEnumValueAsMaskValueInEditor = "true"))
enum class EItemFeature : uint8
{
	None UMETA(Hidden),
	Locked UMETA(DisplayName = "已锁定"),
	Discardable UMETA(DisplayName = "可丢弃"),
	Tradable UMETA(DisplayName = "可交易"),
	USable UMETA(DisplayName = "可使用"),
	Interactable UMETA(DisplayName = "可交互"),
	Equipable UMETA(DisplayName = "可装备"),
	Combinable UMETA(DisplayName = "可合成"),
};

ENUM_CLASS_FLAGS(EItemFeature);

