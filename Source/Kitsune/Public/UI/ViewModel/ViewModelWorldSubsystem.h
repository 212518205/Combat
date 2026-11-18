#pragma once

#include "CoreMinimal.h"
#include "ViewModelBase.h"
#include "Subsystems/WorldSubsystem.h"
#include "ViewModelWorldSubsystem.generated.h"

class UViewModelBase;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnViewModelReadyDelegate, AActor* /*ObserverActor*/, UViewModelBase* /*VM*/);

/**
 *
 */
UCLASS()
class KITSUNE_API UViewModelWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UViewModelWorldSubsystem* Get(const UObject* WorldContextObject);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	template <class T>
	T* GetOrCreate(AActor* Owner);
	UViewModelBase* GetOrCreateByClass(AActor* Owner, TSubclassOf<UViewModelBase> ViewModelClass);

	template <class T>
	T* Find(AActor* Owner) const;
	UFUNCTION(BlueprintCallable, Category = "ViewModel", meta = (DeterminesOutputType = "ViewModelClass"))
	UViewModelBase* FindByClass(AActor* Owner, TSubclassOf<UViewModelBase> ViewModelClass) const;

	void Remove(AActor* Owner);

	FOnViewModelReadyDelegate OnViewModelReady;
	void NotifyViewModelReady(UViewModelBase* ViewModel) const;

protected:
	void HandleActorDestroyed(AActor* DestroyedActor);

	FDelegateHandle ActorDestroyedHandle;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UViewModelBase>> ViewModels;
};

template <class T>
T* UViewModelWorldSubsystem::GetOrCreate(AActor* Owner)
{
	return static_cast<T*>(GetOrCreateByClass(Owner, T::StaticClass()));
}

template <class T>
T* UViewModelWorldSubsystem::Find(AActor* Owner) const
{
	if (!IsValid(Owner)) return nullptr;

	for (const TObjectPtr<UViewModelBase>& Entry : ViewModels)
	{
		UViewModelBase* ViewModel = Entry.Get();
		if (!IsValid(ViewModel)) continue;
		if (ViewModel->GetOwnerActor() != Owner) continue;
		if (ViewModel->GetClass() != T::StaticClass()) continue;

		return static_cast<T*>(ViewModel);
	}
	return nullptr;
}
