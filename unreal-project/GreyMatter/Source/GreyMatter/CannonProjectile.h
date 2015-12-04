// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CannonProjectile.generated.h"

UCLASS()
class GREYMATTER_API ACannonProjectile : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	ACannonProjectile();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *ourVisualComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void ImpulseProjectileOnVector(FVector forward, float magnitude = 3000.0f);
};
