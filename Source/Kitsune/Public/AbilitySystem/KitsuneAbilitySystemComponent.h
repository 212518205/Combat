#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CommonTypes/EnumTypes.h"
#include "KitsuneAbilitySystemComponent.generated.h"

/**
 * 
 */

UCLASS()
class KITSUNE_API UKitsuneAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/*** `@BC`   描述: 输入触发依据InputTag激活对应能力   `BC@` ***/
	void OnAbilityInputPressed(const FGameplayTag& InputTag);
	void OnAbilityInputReleased(const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	bool TryActivateAbilityByTag(FGameplayTag ActivateAbilityTag);
	
protected:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	
	void NotifyAbilityChangedToUI(const FGameplayAbilitySpec& AbilitySpec, const EAbilityChangeType AbilityChangedType) const;
	
};
