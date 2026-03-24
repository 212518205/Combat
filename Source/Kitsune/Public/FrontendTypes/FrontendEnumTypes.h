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


//UENUM(meta = (Bitflags, UseEnumValueAsMaskValueInEditor = "true"))
//UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
//enum class EItemFeature : uint8
//{
//	Locked UMETA(DisplayName = "已锁定"),
//	Discardable UMETA(DisplayName = "可丢弃"),
//	Tradable UMETA(DisplayName = "可交易"),
//	USable UMETA(DisplayName = "可使用"),
//	Interactable UMETA(DisplayName = "可交互"),
//	Equipable UMETA(DisplayName = "可装备"),
//	Combinable UMETA(DisplayName = "可合成"),
//};
//ENUM_CLASS_FLAGS(EItemFeature);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EItemFeature : uint8
{
	None = 0 UMETA(Hidden),                    
	Locked = 1 << 0 UMETA(DisplayName = "已锁定"),
	Discardable = 1 << 1 UMETA(DisplayName = "可丢弃"),
	Tradable = 1 << 2 UMETA(DisplayName = "可交易"),
	Usable = 1 << 3 UMETA(DisplayName = "可使用"),   // 建议改成 Usable
	Interactable = 1 << 4 UMETA(DisplayName = "可交互"),
	Equipable = 1 << 5 UMETA(DisplayName = "可装备"),
	Combinable = 1 << 6 UMETA(DisplayName = "可合成"),
};
ENUM_CLASS_FLAGS(EItemFeature);

UENUM(BlueprintType)
enum class ETileViewEntryType : uint8
{
	DefaultMenuIcon,
	LongerMenuIcon,
	WiderMenuIcon,
};

