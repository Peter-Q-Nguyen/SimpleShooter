// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "ShooterCharacter.h"
#include "AIController.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController * AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(Pawn);
	if (!ShooterCharacter) return EBTNodeResult::Failed;
	ShooterCharacter->ShootGun();


	return EBTNodeResult::Succeeded;
}
