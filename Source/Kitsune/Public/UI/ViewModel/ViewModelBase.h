#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ViewModelBase.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
struct FModelSet;
/**
 *
 */
UCLASS(Abstract)
class KITSUNE_API UViewModelBase : public UObject
{
	GENERATED_BODY()
	 
public:
	virtual void Initialize(const FModelSet& ModelSet) {}
	virtual void BindCallbacksToDependencies() {}

protected:
	UPROPERTY(BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<APlayerState> PlayerState;
};

