

#include "UI/ViewModel/PlayerViewModel.h"

#include "DebugHelper.h"
#include "AbilitySystem/Abilities/ActiveGameplayAbility.h"
#include "Characters/KitsuneCharacter.h"

void UPlayerViewModel::BeginDestroy()
{
    Super::BeginDestroy();
    
}

bool UPlayerViewModel::NativeInitialize()
{
	const bool bSuperInitSuccess = Super::NativeInitialize();
    
    if (const AKitsuneCharacter* Character = Cast<AKitsuneCharacter>(OwningPawn))
    {
        CarriedInventorySystem = Character->GetInventorySystemComponent();
    }
    if (CarriedInventorySystem)
    {
        CarriedInventorySystem->CapacityChanged.AddDynamic(this, &ThisClass::OnCategoryCapacityChanged);
    }
    
    return bSuperInitSuccess;
}

void UPlayerViewModel::AddInteractableItemInstance(UInventoryItemInstance* ItemInstance)
{
    if (ItemInstance && !OverlappedItemInstances.Contains(ItemInstance))
    {
        OverlappedItemInstances.Add(ItemInstance);
        OnInteractableItemChange.Broadcast(ItemInstance, EItemInstanceAction::EAddInstance);
    }
}

void UPlayerViewModel::RemoveInteractableItemInstance(UInventoryItemInstance* ItemInstance)
{
    if (ItemInstance && OverlappedItemInstances.Contains(ItemInstance))
    {
        OverlappedItemInstances.Remove(ItemInstance);
        OnInteractableItemChange.Broadcast(ItemInstance, EItemInstanceAction::ERemoveInstance);
    }
}

void UPlayerViewModel::SetIsSensing(const bool InIsSensing)
{
    if (bIsSensing != InIsSensing)
    {
        bIsSensing = InIsSensing;
        OnReticleStateChanged.Broadcast();
    }
}

void UPlayerViewModel::SetPlayerWeaponIcon(const TSoftObjectPtr<UTexture2D> InWeaponIcon)
{
	WeaponIcon = InWeaponIcon;
	OnWeaponChanged.Broadcast();
}

void UPlayerViewModel::UpdateAbilityList(FGameplayAbilitySpec Spec, const EAbilityChangeType ChangedType)
{    
    switch (ChangedType)
    {
    case EAbilityChangeType::AddAbility:
        {
            if (!AbilityUIData.ContainsByPredicate([Spec](const FAbilityUIData& Entry){return Spec.Handle == Entry.SpecHandle;}))
            {
                if (UActiveGameplayAbility* ActiveAbility = Cast<UActiveGameplayAbility>(Spec.Ability))
                {
                    FAbilityUIData Data;
                    Data.SpecHandle = Spec.Handle;
                    Data.DisplayName = ActiveAbility->GetDisplayName();
                    Data.AbilityTriggerKeyIcon = ActiveAbility->GetAbilityTriggerKeyIcon();
                    Data.Icon = ActiveAbility->GetAbilityIcon();
                    Data.Priority = ActiveAbility->GetPriority();
                    Data.CooldownTag = ActiveAbility->GetCooldownTag();
                    AbilityUIData.Add(Data);
                }
            }
        }
        break;
        
    case EAbilityChangeType::RemoveAbility:
        {
            int32 RemoveIndex = AbilityUIData.IndexOfByPredicate([Spec](const FAbilityUIData& Entry) {
                return Spec.Handle == Entry.SpecHandle;
            });

            if (RemoveIndex != INDEX_NONE)
            {
                AbilityUIData.RemoveAt(RemoveIndex);
            }
            else
            {
                Debug::Print(TEXT("RemoveAbility: SpecHandle not found in UI list"));
            }
        }
        break;
        
    default:
        break;
    }
    
    AbilityUIData.Sort([](const FAbilityUIData& A, const FAbilityUIData& B)
    {
       return A.Priority <  B.Priority;
    });
     
    OnGameplayAbilityChanged.Broadcast(Spec.Handle);
}

TArray<TPair<FName, FInventoryCategoryGroup>> UPlayerViewModel::GetAllInventoryCategories() const
{
	return CarriedInventorySystem ? CarriedInventorySystem->GetAllCategoryItem() : TArray<TPair<FName, FInventoryCategoryGroup>>();
}

TArray<UInventorySlotData*> UPlayerViewModel::GetInventoryItemsByCategory(const FName CategoryID) const
{
	return CarriedInventorySystem ? CarriedInventorySystem->GetAllItemsByCategory(CategoryID) : TArray<UInventorySlotData*>();
}

int32 UPlayerViewModel::GetCategoryCapacity(const FName CategoryID) const
{
    return CarriedInventorySystem ? CarriedInventorySystem->GetCapacityByCategoryID(CategoryID) : 0;
}

TArray<FAbilityUIData> UPlayerViewModel::GetPlayerAbilities() const
{
    return AbilityUIData;
}

FAbilityUIData UPlayerViewModel::GetAbilityUIDataByHandle(const FGameplayAbilitySpecHandle& SpecHandle)
{
    if (const int32 Index = AbilityUIData.IndexOfByPredicate([SpecHandle](const FAbilityUIData& Entry)
    {
        return SpecHandle == Entry.SpecHandle;
    }); Index != INDEX_NONE)
    {
        return AbilityUIData[Index];
    }
    
    return FAbilityUIData();
}

void UPlayerViewModel::OnCategoryCapacityChanged(const FName CategoryID, const int32 CategoryCapacity)
{
    CategoryCapacityChanged.Broadcast(CategoryID, CategoryCapacity);
}
