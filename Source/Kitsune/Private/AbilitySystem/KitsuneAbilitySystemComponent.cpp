#include "AbilitySystem/KitsuneAbilitySystemComponent.h"

#include "DebugHelper.h"
#include "UIManagerSubsystem.h"
#include "AbilitySystem/Abilities/ActiveGameplayAbility.h"
#include "AbilitySystem/Abilities/KitsuneGameplayAbility.h"
#include "Characters/EnemyCharacter.h"
#include "UI/ViewModel/PlayerViewModel.h"


void UKitsuneAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	for (auto& AbilitySpec:GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))continue;

		if (!AbilitySpec.IsActive())
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UKitsuneAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
	 
}

bool UKitsuneAbilitySystemComponent::TryActivateAbilityByTag(const FGameplayTag ActivateAbilityTag)
{
	check(ActivateAbilityTag.IsValid());

	TArray<FGameplayAbilitySpec*> FoundAbilitySpec;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(ActivateAbilityTag.GetSingleTagContainer(), FoundAbilitySpec);

	if (!FoundAbilitySpec.IsEmpty())
	{
		const int32 ActivatableAbilityIndex = FMath::RandRange(0, FoundAbilitySpec.Num() - 1);

		if (const FGameplayAbilitySpec* SpecToActive = FoundAbilitySpec[ActivatableAbilityIndex]; !SpecToActive->IsActive())
		{
			return TryActivateAbility(SpecToActive->Handle);
		}

	}

	return false;
}

void UKitsuneAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);
	
	if (AbilitySpec.Ability.IsA(UActiveGameplayAbility::StaticClass()))
	{
		NotifyAbilityChangedToUI(AbilitySpec, EAbilityChangeType::AddAbility);
	}
}

void UKitsuneAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnRemoveAbility(AbilitySpec);
	
	if (AbilitySpec.Ability.IsA(UActiveGameplayAbility::StaticClass()))
	{
		NotifyAbilityChangedToUI(AbilitySpec, EAbilityChangeType::RemoveAbility);
	}
}

void UKitsuneAbilitySystemComponent::NotifyAbilityChangedToUI(const FGameplayAbilitySpec& AbilitySpec,
	const EAbilityChangeType AbilityChangedType) const
{
	if (const UWorld* World = GetWorld(); !World || World->bIsTearingDown)
	{
		return;
	}
	
	if (AbilitySpec.Ability.IsA(UActiveGameplayAbility::StaticClass()))
	{
		if (UUIManagerSubsystem* UIManager = UUIManagerSubsystem::GetUIManager(GetAvatarActor()))
		{
			if (UPlayerViewModel* PlayerVM = UIManager->TryGetViewModel<UPlayerViewModel>(GetAvatarActor()))
			{
				PlayerVM->UpdateAbilityList(AbilitySpec, AbilityChangedType);
			}
		}
	}
}

