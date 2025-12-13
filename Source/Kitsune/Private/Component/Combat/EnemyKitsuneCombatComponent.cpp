// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Combat/EnemyKitsuneCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FrontendDebugHelper.h"
#include "GameplayTag/KitsuneGameplayTag.h"

void UEnemyKitsuneCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))return;

	OverlappedActors.AddUnique(HitActor);

	/*** TODO: 添加卸势检测... [2025年12月2日 22:23:59 来自`@BC@`] ***/
	bool bIsForceRelieved = false;

	const bool bIsPlayerForceRelieved = false;
	const bool bIsMyAttackUnForceRelieved = false;

	if (bIsPlayerForceRelieved && bIsMyAttackUnForceRelieved)
	{
		Debug::Print(TEXT("成功卸势"));
		/*** TODO: 检查卸势是否有效... [2025年12月2日 22:30:24 来自`@BC@`] ***/
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	if (bIsForceRelieved)
	{
		/*** TODO: 成功卸势回调... [2025年12月2日 22:32:01 来自`@BC@`] ***/
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			KitsuneGameplayTags::Shared_Event_MeleeHit,
			EventData
		);
	}
}

void UEnemyKitsuneCombatComponent::OnPulledFromTargetActor(AActor* InteractedActor)
{
}
