// Fill out your copyright notice in the Description page of Project Settings.


#include "KillAllGameModeBase.h"

void AKillAllGameModeBase::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);
	UE_LOG(LogTemp, Warning, TEXT("PawnKilled"));



	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast Success"))
		PlayerController->GameHasEnded(nullptr, false);
	}

}