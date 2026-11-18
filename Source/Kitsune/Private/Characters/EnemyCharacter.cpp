#include "Characters/EnemyCharacter.h"

#include "AbilitySystem/KitsuneAbilitySystemComponent.h"
#include "AbilitySystem/KitsuneAttributeSet.h"
#include "Characters/Data/DataAssetStartDataEnemy.h"
#include "Component/Combat/EnemyKitsuneCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
// [VMREFACTOR] 去掉 UIManagerSubsystem.h 依赖
#include "UI/ViewModel/ASCViewModel.h"
#include "UI/ViewModel/ViewModelWorldSubsystem.h"

AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComp = CreateDefaultSubobject<UKitsuneAbilitySystemComponent>(
		"AbilitySystemComponent");
	AbilitySystemComp->SetIsReplicated(true);
	AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UKitsuneAttributeSet>("AttributeSet");

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	CombatComponent = CreateDefaultSubobject<UEnemyKitsuneCombatComponent>(TEXT("CombatComponent"));

	SetNetUpdateFrequency(100.f);
}

void AEnemyCharacter::InitAbilityInfo()
{
	Super::InitAbilityInfo();
	
	AbilitySystemComp->InitAbilityActorInfo(this, this);

	// [VMREFACTOR] 敌人的 ASC 就在自己身上：注册 key = Bind 源 = this
	// [VMREFACTOR] 非 DS 全建（血条逻辑与玩家一致）
	if (UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this))
	{
		VMReg->GetOrCreate<UASCViewModel>(this)->BindAbilitySystemComponent(
			Cast<UKitsuneAbilitySystemComponent>(AbilitySystemComp));
	}

	if (GetNetMode() != NM_DedicatedServer)
	{
		OnWidgetComponentInitialized();
	}

	if (HasAuthority())
	{
		StartData->InitAbilityAndEffectToASC(GetAbilitySystemComponent(), 1);
		ActorTeamID = FGenericTeamId(static_cast<uint8>(EKitsuneTeamID::TeamB));
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityInfo();
}

UKitsuneCombatComponent* AEnemyCharacter::GetKitsuneCombatComponent() const
{
	return CombatComponent;
}

UASCViewModel* AEnemyCharacter::GetEnemyViewModel()
{
	// [VMREFACTOR] 统一走 Registry，角色上不再自己缓存一份
	UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this);
	return VMReg ? VMReg->Find<UASCViewModel>(this) : nullptr;
}
