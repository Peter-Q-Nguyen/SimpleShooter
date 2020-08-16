// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class SIMPLESHOOTER_API APickup : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CollisionVolume;

	/** Base Mesh Component*/
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere,  BlueprintReadWrite, Category = "Configuration", meta = (AllowPrivateAccess = "true"))
	float RotationRate = 10;

	UPROPERTY(EditAnywhere)
	int32 AmmoValue = 5;

	FTimerHandle RespawnTimer;
	UPROPERTY(EditAnywhere)
	int32 RespawnTime = 5;

	void HidePickup();
	void ShowPickup();

public:	
	// Sets default values for this actor's properties
	APickup();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
