#include "UI/ViewModel/ViewModelWorldSubsystem.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "UI/ViewModel/ViewModelBase.h"

namespace
{
	UViewModelBase* FindOwnedViewModel(const TArray<TObjectPtr<UViewModelBase>>& InViewModels, const AActor* Owner, const UClass* ViewModelClass)
	{
		if (!IsValid(Owner)) return nullptr;

		for (const TObjectPtr<UViewModelBase>& Entry : InViewModels)
		{
			UViewModelBase* ViewModel = Entry.Get();
			if (!IsValid(ViewModel)) continue;
			if (ViewModel->GetOwnerActor() != Owner) continue;
			if (IsValid(ViewModelClass) && ViewModel->GetClass() != ViewModelClass) continue;

			return ViewModel;
		}
		return nullptr;
	}
}

UViewModelWorldSubsystem* UViewModelWorldSubsystem::Get(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine
		? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)
		: nullptr;

	return World ? World->GetSubsystem<UViewModelWorldSubsystem>() : nullptr;
}

bool UViewModelWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UWorld* World = Cast<UWorld>(Outer);
	return World != nullptr && World->GetNetMode() != NM_DedicatedServer;
}

void UViewModelWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	ActorDestroyedHandle = World->AddOnActorDestroyedHandler(
		FOnActorDestroyed::FDelegate::CreateUObject(this, &ThisClass::HandleActorDestroyed));
}

void UViewModelWorldSubsystem::Deinitialize()
{
	if (const UWorld* World = GetWorld(); IsValid(World) && ActorDestroyedHandle.IsValid())
	{
		World->RemoveOnActorDestroyedHandler(ActorDestroyedHandle);
		ActorDestroyedHandle.Reset();
	}

	for (const TObjectPtr<UViewModelBase>& Entry : ViewModels)
	{
		UViewModelBase* ViewModel = Entry.Get();
		if (!IsValid(ViewModel)) continue;

		ViewModel->Deinitialize();
	}

	ViewModels.Empty();
	OnViewModelReady.Clear();

	Super::Deinitialize();
}

UViewModelBase* UViewModelWorldSubsystem::GetOrCreateByClass(AActor* Owner, const TSubclassOf<UViewModelBase> ViewModelClass)
{
	if (!IsValid(Owner) || !ViewModelClass) return nullptr;

	if (UViewModelBase* Existing = FindOwnedViewModel(ViewModels, Owner, ViewModelClass))
	{
		return Existing;
	}

	UViewModelBase* ViewModel = NewObject<UViewModelBase>(Owner, ViewModelClass);
	if (!IsValid(ViewModel)) return nullptr;

	ViewModel->InitializeContext(Owner);
	ViewModels.Add(ViewModel);

	return ViewModel;
}

UViewModelBase* UViewModelWorldSubsystem::FindByClass(AActor* Owner, const TSubclassOf<UViewModelBase> ViewModelClass) const
{
	return FindOwnedViewModel(ViewModels, Owner, ViewModelClass);
}

void UViewModelWorldSubsystem::NotifyViewModelReady(UViewModelBase* ViewModel) const
{
	OnViewModelReady.Broadcast(IsValid(ViewModel) ? ViewModel->GetOwnerActor() : nullptr, ViewModel);
}

void UViewModelWorldSubsystem::Remove(AActor* Owner)
{
	if (!IsValid(Owner)) return;

	for (const TObjectPtr<UViewModelBase>& Entry : ViewModels)
	{
		UViewModelBase* ViewModel = Entry.Get();
		if (!IsValid(ViewModel)) continue;
		if (ViewModel->GetOwnerActor() != Owner) continue;

		ViewModel->Deinitialize();
	}

	ViewModels.RemoveAll([Owner](const TObjectPtr<UViewModelBase>& Entry)
	{
		const UViewModelBase* ViewModel = Entry.Get();
		if (!IsValid(ViewModel)) return true;
		return ViewModel->GetOwnerActor() == Owner;
	});
}

void UViewModelWorldSubsystem::HandleActorDestroyed(AActor* DestroyedActor)
{
	if (!IsValid(DestroyedActor)) return;

	Remove(DestroyedActor);
}
