// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendDebugHelper.h"
#include "Components/ActorComponent.h"
#include "KitsuneExtensionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KITSUNE_API UKitsuneExtensionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	template<class T>
	T* GetOwningPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "类型: T 不是APwn的派生子类");
		return CastChecked<T>(GetOwner());
	}

	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template<class T>
	T* GetOwningController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "类型: T 不是AController的派生子类");
		return GetOwningPawn<APawn>()->GetController<T>();
	}
	
	template<typename OwnerType, typename PropertyType, typename NewValueType, typename OnRepType = std::nullptr_t>
	FORCEINLINE bool SetReplicatedProperty(
		OwnerType* Owner,
		PropertyType& Property,
		NewValueType&& NewValue,
		OnRepType OnRep = nullptr)
	{
		const APawn* Pawn = GetOwningPawn();
		if (!IsValid(Pawn))
		{
			Debug::Print(TEXT("SetReplicatedProperty: 无法获取有效的 OwningPawn"));
			return false;
		}

		if (!Pawn->HasAuthority())
		{
			Debug::Print(TEXT("SetReplicatedProperty: 客户端尝试设置，已拒绝"));
			return true;                 
		}
		if (Property == NewValue)return false;
		
		const auto OldProperty = Property;
		Property = std::forward<NewValueType>(NewValue);

		if (OnRep) 
		{
			if (const UWorld* World = Pawn->GetWorld())
			{
				if (const ENetMode NetMode = World->GetNetMode(); NetMode == NM_Standalone ||
					(NetMode == NM_ListenServer && Pawn->IsLocallyControlled()))
				{
					(Owner->*OnRep)(OldProperty);
				}
			}
		}

		return true;
	}
		
};
