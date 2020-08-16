// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Components/SphereComponent.h"
#include "ShooterCharacter.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionVolume;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapBegin);
	//Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
	
}

void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor);
	if (Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("CastSuccess"));
		if (Character->HasMaxAmmo()) return;

		Character->IncrementAmmo(AmmoValue);

		GetWorldTimerManager().SetTimer(RespawnTimer, this, &APickup::ShowPickup, RespawnTime);
		HidePickup();
	}
}

void APickup::HidePickup()
{
	CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetHiddenInGame(true);
}

void APickup::ShowPickup()
{
	CollisionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetHiddenInGame(false);
}
// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += DeltaTime * RotationRate;
	SetActorRotation(Rotation);
}

