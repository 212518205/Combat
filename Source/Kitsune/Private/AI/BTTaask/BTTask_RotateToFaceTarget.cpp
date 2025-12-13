// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaask/BTTask_RotateToFaceTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
	NodeName = TEXT("原生面向目标Actor的旋转");

	AnglePrecision = 10.f;
	RotationInterpSpeed = 5.f;

	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;

	INIT_TASK_NODE_NOTIFY_FLAGS();

	TargetToFaceKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetToFaceKey), AActor::StaticClass());

}

FString UBTTask_RotateToFaceTarget::GetStaticDescription() const
{
	const FString KeyDescription = TargetToFaceKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("平滑旋转直至面向%s，直到角度精度达到%f及以内"), *KeyDescription, AnglePrecision);
}

uint16 UBTTask_RotateToFaceTarget::GetInstanceMemorySize() const
{
	return sizeof(FRotateFaceTargetTaskMemory);
}

void UBTTask_RotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		TargetToFaceKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UBTTask_RotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetToFaceKey.SelectedKeyName));
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	FRotateFaceTargetTaskMemory* Memory = reinterpret_cast<FRotateFaceTargetTaskMemory*>(NodeMemory);
	check(Memory);

	Memory->OwningPawn = OwningPawn;
	Memory->TargetActor = TargetActor;

	if (!Memory->IsValid())
	{
		return EBTNodeResult::Failed;
	}
	if (HasReachedAnglePrecision(OwningPawn, TargetActor))
	{
		Memory->Reset();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_RotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const FRotateFaceTargetTaskMemory* Memory = reinterpret_cast<FRotateFaceTargetTaskMemory*>(NodeMemory);

	if (!Memory->IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (HasReachedAnglePrecision(Memory->OwningPawn.Get(), Memory->TargetActor.Get()))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		const FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(Memory->OwningPawn->GetActorLocation(), Memory->TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(Memory->OwningPawn->GetActorRotation(), LookRot, DeltaSeconds, RotationInterpSpeed);

		Memory->OwningPawn->SetActorRotation(TargetRot);
	}
}

bool UBTTask_RotateToFaceTarget::HasReachedAnglePrecision(const APawn* QueryPawn, const AActor* TargetActor) const
{

	if ((!QueryPawn) || (!TargetActor))return false;

	const FVector OwnerForward = QueryPawn->GetActorForwardVector();
	const FVector ToTargetNormalize = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(OwnerForward, ToTargetNormalize);

	return DotResult >= FMath::Cos(FMath::DegreesToRadians(AnglePrecision));
}

