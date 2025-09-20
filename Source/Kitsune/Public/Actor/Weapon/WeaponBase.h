// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/ItemBase.h"
#include "WeaponBase.generated.h"


class UBoxComponent;

/**
 * 
 */
UCLASS()
class KITSUNE_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

public:
	AWeaponBase();

protected:
	/*** `@BC`   描述: 	基类基础上添加盒体检测   `BC@` ***/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UBoxComponent> ItemOverlapBox;
};
