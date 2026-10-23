#include "AbilitySystem/KitsuneAbilitySystemComponent.h"
#include "UIManagerSubsystem.h"
#include "AbilitySystem/Abilities/KitsuneGameplayAbility.h"
#include "UI/ViewModel/PlayerViewModel.h"


void UKitsuneAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!MatchesInputTag(AbilitySpec, InputTag))continue;

		if (AbilitySpec.IsActive())
		{
			AbilitySpecInputPressed(AbilitySpec);
		}
		else
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UKitsuneAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (!MatchesInputTag(Spec, InputTag))continue;
		
		if (Spec.IsActive())
		{
			AbilitySpecInputReleased(Spec);
		}
	}

}

bool UKitsuneAbilitySystemComponent::TryActivateAbilityByTag(const FGameplayTag ActivateAbilityTag)
{
	check(ActivateAbilityTag.IsValid());

	TArray<FGameplayAbilitySpec*> FoundAbilitySpec;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(ActivateAbilityTag.GetSingleTagContainer(), FoundAbilitySpec);

	if (!FoundAbilitySpec.IsEmpty())
	{
		const int32 ActivatableAbilityIndex = FMath::RandRange(0, FoundAbilitySpec.Num() - 1);

		if (const FGameplayAbilitySpec* SpecToActive = FoundAbilitySpec[ActivatableAbilityIndex]; !SpecToActive->
			IsActive())
		{
			return TryActivateAbility(SpecToActive->Handle);
		}
	}

	return false;
}

void UKitsuneAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	if (const UKitsuneGameplayAbility* Ability = Cast<UKitsuneGameplayAbility>(AbilitySpec.Ability))
	{
		if (Ability->AbilityTypeTag != KitsuneGameplayTags::Ability_Type_Hidden)
		{
			OnAbilityNumChanged.Broadcast(AbilitySpec, EAbilityChangeType::AddAbility);
		}
	}
}

void UKitsuneAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnRemoveAbility(AbilitySpec);

	if (const UKitsuneGameplayAbility* Ability = Cast<UKitsuneGameplayAbility>(AbilitySpec.Ability))
	{
		if (Ability->AbilityTypeTag != KitsuneGameplayTags::Ability_Type_Hidden)
		{
			OnAbilityNumChanged.Broadcast(AbilitySpec, EAbilityChangeType::RemoveAbility);
		}
	}
}

bool UKitsuneAbilitySystemComponent::MatchesInputTag(const FGameplayAbilitySpec& Spec, const FGameplayTag& InputTag)
{
	return Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag);
}
