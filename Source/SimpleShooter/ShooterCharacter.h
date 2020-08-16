// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	void ShootGun();
	UFUNCTION(BlueprintPure)
	int32 GetAmmo();

	void IncrementAmmo(int32 Ammo);

	bool HasMaxAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	void JumpPressed();
	void JumpReleased();

	void WeaponSwapUp();
	void WeaponSwapDown();

	void SetGun(int32 Index);

	UPROPERTY(EditAnywhere)
	float RotationRate = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	TArray < TSubclassOf<AGun>> GunClasses;

	UPROPERTY(VisibleAnywhere)
	AGun* CurrentGun;

	//UPROPERTY(EditDefaultsOnly)
	TArray<AGun*>GunArray;
	UPROPERTY(VisibleAnywhere)
	int32 GunIndex = 0;


};
