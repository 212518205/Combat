#pragma once

#include "CoreMinimal.h"
#include "ViewModelBase.generated.h"

/**
 *
 */
UCLASS(Abstract)
class KITSUNE_API UViewModelBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void InitializeContext(AActor* InOwner);

	virtual void Deinitialize();

	bool IsViewModelReady() const { return bReady; }
	AActor* GetOwnerActor() const { return CachedOwner.Get(); }

protected:
	void MarkReady();

	TWeakObjectPtr<AActor> CachedOwner;

	bool bReady = false;
};
