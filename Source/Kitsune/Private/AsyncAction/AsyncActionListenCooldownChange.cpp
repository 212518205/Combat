#include "AsyncAction/AsyncActionListenCooldownChange.h"

#include "AbilitySystem/KitsuneAbilitySystemComponent.h"

UAsyncActionListenCooldownChange* UAsyncActionListenCooldownChange::ListenCooldownChange(UObject* WorldContextObject,
                                                                   UKitsuneAbilitySystemComponent* ASC, UPARAM(meta = (Categories = "UI.Widget")) FGameplayTag InCooldownTag, const FGameplayAbilitySpecHandle Handle)
{
	if (!ASC || !InCooldownTag.IsValid() || !Handle.IsValid())return nullptr;
	
	UAsyncActionListenCooldownChange* Node = NewObject<UAsyncActionListenCooldownChange>();
	Node->CachedASC = ASC;
	Node->CachedCooldownTag = InCooldownTag;
	Node->CachedAbilitySpecHandle = Handle;
	
	return Node;
}

void UAsyncActionListenCooldownChange::Activate()
{
	Super::Activate();
	
	if (!CachedASC.IsValid())
	{
		EndTask();
		return;
	}
	
	UKitsuneAbilitySystemComponent* ASCPtr = CachedASC.Get();
	TagChangeDelegateHandle = ASCPtr->RegisterGameplayTagEvent(CachedCooldownTag, EGameplayTagEventType::NewOrRemoved).
	                                  AddUObject(this, &ThisClass::OnCooldownTagChanged);
	
	if (ASCPtr->HasMatchingGameplayTag(CachedCooldownTag))
	{
		OnCooldownTagChanged(CachedCooldownTag, 1);
	}
}

void UAsyncActionListenCooldownChange::EndTask()
{
	if (CachedASC.IsValid())
	{
		UKitsuneAbilitySystemComponent* ASCPtr = CachedASC.Get();
		if (TagChangeDelegateHandle.IsValid())
		{
			ASCPtr->RegisterGameplayTagEvent(CachedCooldownTag, EGameplayTagEventType::NewOrRemoved).Remove(TagChangeDelegateHandle);
			TagChangeDelegateHandle.Reset();
		}
	}
	
	SetReadyToDestroy();
}

void UAsyncActionListenCooldownChange::OnCooldownTagChanged(const FGameplayTag Tag, int32 Count)
{
	if (!CachedASC.IsValid())
	{
		EndTask();
		return;
	}
	
	UKitsuneAbilitySystemComponent* ASCPtr = CachedASC.Get();
	if (!ASCPtr)
	{
		EndTask();
		return;
	}
	if (Count > 0)
	{
		FGameplayEffectQuery Query;
		Query.CustomMatchDelegate = FActiveGameplayEffectQueryCustomMatch::CreateLambda(
			[this, ASCPtr](const FActiveGameplayEffect& Effect)
			{
				FGameplayTagContainer AssetTags;
				FGameplayTagContainer GrantTags;
				Effect.Spec.GetAllAssetTags(AssetTags);
				Effect.Spec.GetAllGrantedTags(GrantTags);
				if (const bool bHasCooldownTag = AssetTags.HasTag(CachedCooldownTag) || GrantTags.HasTag(CachedCooldownTag); !bHasCooldownTag)return false;
				
				if (CachedAbilitySpecHandle.IsValid())
				{
					const FGameplayAbilitySpec* Spec = ASCPtr->FindAbilitySpecFromHandle(CachedAbilitySpecHandle);
					if (!Spec || !Spec->Ability)
					{	
						return false;
					}
					const UGameplayAbility* OurAbility = Spec->Ability;
					const UGameplayAbility* SpecAbility = Effect.Spec.GetContext().GetAbility();
					return OurAbility && SpecAbility && OurAbility == SpecAbility;
				}
				
				return true;
			});
		TArray<TPair<float, float>> DurationAndTimeRemaining = ASCPtr->GetActiveEffectsTimeRemainingAndDuration(Query);
		float MaxTimeRemaining = 0.f;
		float MaxDuration = 0.f;
		for (auto& [TimeRemaining, Duration] : DurationAndTimeRemaining)
		{
			if (MaxTimeRemaining < TimeRemaining)
			{
				MaxTimeRemaining = TimeRemaining;
				MaxDuration = Duration;
			}
		}
		
		CooldownStart.Broadcast(MaxTimeRemaining, MaxDuration);
	}
	else
	{
		CooldownEnd.Broadcast(0.f);
		EndTask();
	}
}














