// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	//Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	//Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	//Gun->SetOwner(this);

	for (TSubclassOf<AGun> GunClass : GunClasses)
	{
		AGun* Gun = GetWorld()->SpawnActor<AGun>(GunClass);
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->SetOwner(this);
		GunArray.Push(Gun);
	}

	if (GunArray.Num() > 0)
	{
		SetGun(GunIndex);
	}
	Health = MaxHealth;

}


// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AShooterCharacter::JumpPressed);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AShooterCharacter::JumpReleased);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Released, this, &AShooterCharacter::ShootGun);

	PlayerInputComponent->BindAction(TEXT("WeaponSwapUp"), EInputEvent::IE_Released, this, &AShooterCharacter::WeaponSwapUp);
	PlayerInputComponent->BindAction(TEXT("WeaponSwapDown"), EInputEvent::IE_Released, this, &AShooterCharacter::WeaponSwapDown);
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageToApply = FMath::Min(Health, DamageToApply);

	Health -= DamageToApply;

	//UE_LOG(LogTemp, Warning, TEXT("Health: %f "), Health);

	if (IsDead())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ASimpleShooterGameModeBase * GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
	}

	return DamageToApply;

}

void AShooterCharacter::SetGun(int32 Index)
{
	GunIndex = Index;
	if (GunIndex < 0 || GunIndex >= GunArray.Num())
	{
		GunIndex = 0;
	}

	UE_LOG(LogTemp, Warning, TEXT("Setting Gun to %i"), GunIndex);

	int Count = 0;
	for (AGun* Gun : GunArray)
	{
		if (Count == GunIndex)
		{
			//		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
			Gun->SetActorHiddenInGame(false);
			CurrentGun = Gun;
		}
		else
		{
			Gun->SetActorHiddenInGame(true);

		}
		
		Count++;
	}

}

bool AShooterCharacter::IsDead() const
{
	return (Health <= 0);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::JumpPressed()
{
	Jump();
}

void AShooterCharacter::JumpReleased()
{

}

void AShooterCharacter::WeaponSwapUp()
{
	GunIndex++;
	if (GunIndex >= GunClasses.Num())
	{
		GunIndex = 0;
	}
	SetGun(GunIndex);
}


void AShooterCharacter::WeaponSwapDown()
{
	GunIndex--;
	if (GunIndex < 0)
		GunIndex = GunArray.Num() - 1;
	SetGun(GunIndex);
}


float AShooterCharacter::GetHealthPercent() const
{
//	float percent = (1 / MaxHealth) * Health;
//	return FMath::Clamp(percent, 0.f, 1.f);

	return Health / MaxHealth;
}

void AShooterCharacter::ShootGun()
{
	if (CurrentGun != nullptr)
		CurrentGun->PullTrigger();
}

int32 AShooterCharacter::GetAmmo()
{
	if (CurrentGun != nullptr)
		return CurrentGun->GetAmmo();
	else
		return 0;
}


void AShooterCharacter::IncrementAmmo(int32 Ammo)
{
	if (CurrentGun != nullptr)
	CurrentGun->IncrementAmmo(Ammo);
}

bool AShooterCharacter::HasMaxAmmo()
{
	if (CurrentGun != nullptr)
		return CurrentGun->HasMaxAmmo();
	else
		return true;
}