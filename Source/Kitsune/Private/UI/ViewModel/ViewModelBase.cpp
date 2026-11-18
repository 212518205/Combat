#include "UI/ViewModel/ViewModelBase.h"

#include "UI/ViewModel/ViewModelWorldSubsystem.h"

void UViewModelBase::InitializeContext(AActor* InOwner)
{
	CachedOwner = InOwner;
	bReady = false;
}

void UViewModelBase::Deinitialize()
{
	bReady = false;
}

void UViewModelBase::MarkReady()
{
	bReady = true;

	if (UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this))
	{
		VMReg->NotifyViewModelReady(this);
	}
}
