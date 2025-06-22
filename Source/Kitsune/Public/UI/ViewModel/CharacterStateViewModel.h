#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "CharacterStateViewModel.generated.h"

struct FOnAttributeChangeData;

/**
 *
 */
UCLASS(BlueprintType)
class KITSUNE_API UCharacterStateViewModel : public UViewModelBase
{
	GENERATED_BODY()

public:
	/*Public Function Begin*/
	virtual void Initialize(const FModelSet& ModelSet) override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintPure,Category="GAS|Attributes")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure,Category="GAS|Attributes")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure,Category="GAS|Attribute")
	float GetHealthPercent() const { return MaxHealth > 0 ? Health / MaxHealth : 0; }
	/*Public Function End*/

protected:
	void HealthChanged(const FOnAttributeChangeData& Data) ;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) ;

private:
	UPROPERTY()
	float Health;

	UPROPERTY()
	float MaxHealth;

};

