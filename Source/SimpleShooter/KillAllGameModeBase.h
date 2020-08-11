// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "KillAllGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AKillAllGameModeBase : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()
	
public:
	void PawnKilled(APawn* PawnKilled) override;
};
