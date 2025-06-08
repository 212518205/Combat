#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "CharacterStateViewModel.generated.h"

struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewHealth);

/**
 *
 */
UCLASS(BlueprintType)
class KITSUNE_API UCharacterStateViewModel : public UViewModelBase
{
	GENERATED_BODY()

public:
	virtual void Initialize(const FModelSet& ModelSet) override;
	virtual void BindCallbacksToDependencies() override;

	~UCharacterStateViewModel(){ UE_LOG(LogTemp, Warning, TEXT("222222222222bbbbbbbbbbb")); }

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UFUNCTION(BlueprintPure,Category="GAS|Attributes")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure,Category="GAS|Attributes")
	float GetMaxHealth() const { return MaxHealth; }

protected:
	void HealthChanged(const FOnAttributeChangeData& Data) ;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) ;

private:
	UPROPERTY()
	float Health;

	UPROPERTY()
	float MaxHealth;

};

