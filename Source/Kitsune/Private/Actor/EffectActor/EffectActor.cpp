// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EffectActor/EffectActor.h"

#include "AbilitySyetem/KitsuneAttributeSet.h"
#include "Characters/KitsuneCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AEffectActor::AEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AEffectActor::EndOverlap);
}

void AEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherCmp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AKitsuneCharacter* KitsuneCharacter=Cast<AKitsuneCharacter>(OtherActor))
	{
		UKitsuneAttributeSet* KitsuneAttributeSet = Cast<UKitsuneAttributeSet>(KitsuneCharacter->GetAttributeSet());
		checkf(KitsuneAttributeSet, TEXT("KitsuneAttributeSet Cast failed"));
		KitsuneAttributeSet->SetHealth(KitsuneAttributeSet->GetHealth() + 25.f);
		KitsuneAttributeSet->SetMaxHealth(KitsuneAttributeSet->GetMaxHealth() + 25.f);
	}
}

void AEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

