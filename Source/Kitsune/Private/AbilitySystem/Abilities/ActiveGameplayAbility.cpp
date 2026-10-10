#include "AbilitySystem/Abilities/ActiveGameplayAbility.h"

#include "MotionWarpingComponent.h"

UPlayerCombatComponent* UActiveGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{	
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPlayerCombatComponent>();
}

UMotionWarpingComponent* UActiveGameplayAbility::GetMotionWarpingComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UMotionWarpingComponent>();
}
