// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}


void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMesh, TEXT("MuzzleFlashSocket"));
	FHitResult HitResult;
	FVector ShotDirection;
	if (GunTrace(HitResult, ShotDirection))
	{
		//DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 20, FColor::Red, true);
	
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactEffect, HitResult.Location, ShotDirection.Rotation());

		AActor* HitTarget = HitResult.GetActor();
		if (HitTarget != nullptr)
		{
			AController* OwnerController = GetOwnerController();
			if (OwnerController == nullptr) return;

			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			HitTarget->TakeDamage(Damage, DamageEvent, OwnerController, this);

		}
	}

}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{


	FVector Location;
	FRotator Rotation;
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;

	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (!OwnerPawn) return nullptr;
	return  OwnerPawn->GetController();
}
