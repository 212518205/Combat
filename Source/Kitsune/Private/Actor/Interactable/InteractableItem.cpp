// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Interactable/InteractableItem.h"

#include "FrontendDebugHelper.h"
#include "Component/Interaction/InteractionComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/PawnInteractInterface.h"
#include "Net/UnrealNetwork.h"

AInteractableItem::AInteractableItem()
{

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetupAttachment(GetRootComponent());

	BoxComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBoxCompBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnBoxCompEndOverlap);
}

void AInteractableItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemInstance);
}


void AInteractableItem::OnBoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IPawnInteractInterface* InteractInterface = Cast<IPawnInteractInterface>(OtherActor))
	{
		if (UInteractionComponent* InteractComp = InteractInterface->GetInteractionComp())
		{
			InteractComp->AddInteractableItem(ItemInstance);
		}
	}
}

void AInteractableItem::OnBoxCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IPawnInteractInterface* InteractInterface = Cast<IPawnInteractInterface>(OtherActor))
	{
		if (UInteractionComponent* InteractComp = InteractInterface->GetInteractionComp())
		{
			InteractComp->RemoveInteractableItem(ItemInstance);
		}
	}
}

