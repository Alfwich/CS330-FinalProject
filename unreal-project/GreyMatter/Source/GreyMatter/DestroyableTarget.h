// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GreyMatter.h"
#include "DestroyableTarget.generated.h"

UCLASS()
class GREYMATTER_API ADestroyableTarget : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestroyableTarget();

	UPROPERTY(EditAnywhere, Category="Mesh")
	UStaticMeshComponent* ourVisualComponent;

	UPROPERTY(EditAnywhere, Category="Mesh")
	UMaterial* ourVisualComponentMaterial;

	UPROPERTY(EditAnywhere, Category="Scoring")
	int32 totalHits;

	UPROPERTY(EditAnywhere, Category="Scoring")
	float scorePerHit;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
  void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
