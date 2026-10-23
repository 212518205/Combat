#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/KitsuneAbilitySystemComponent.h"
#include "AbilitySystem/KitsuneAttributeSet.h"
#include "ViewModelBase.generated.h"

class UInventoryComponent;
class UKitsuneCombatComponent;
/**
 * 
 */
UCLASS(Abstract)
class KITSUNE_API UViewModelBase : public UObject
{
	GENERATED_BODY()

public:
	virtual bool NativeInitialize(APawn* InPawn) { return true; }

	template <class T = UViewModelBase>
	static T* GetViewModel(AController* InController, APawn* InPawn);

protected:	
	virtual void BindCombatComponent(UKitsuneCombatComponent* CombatComp){}
	virtual void BindAbilitySystemComponent(UKitsuneAbilitySystemComponent* ASC){}
	virtual void BindInventorySystemComponent(UInventoryComponent* ISC){}
};

template<class T>
inline T* UViewModelBase::GetViewModel(AController* InController, APawn* InPawn)
{
	if (T* ViewModel = NewObject<T>(); ViewModel->NativeInitialize(InPawn))
	{
		return ViewModel;
	}
	return nullptr;
}
