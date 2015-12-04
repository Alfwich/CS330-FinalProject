// Fill out your copyright notice in the Description page of Project Settings.

#include "GreyMatter.h"
#include "DestroyableTarget.h"


// Sets default values
ADestroyableTarget::ADestroyableTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ourVisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetBody"));
	RootComponent = ourVisualComponent;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
	ourVisualComponentMaterial = LoadObjFromPath<UMaterial>("/Game/GreyMatter/Materials/Target_Mat.Target_Mat");

	if (CylinderVisualAsset.Succeeded()) {
			ourVisualComponent->SetStaticMesh(CylinderVisualAsset.Object);
			ourVisualComponent->SetWorldScale3D(FVector(1.0f));
			ourVisualComponent->SetMaterial(0, ourVisualComponentMaterial);
	}

}

// Called when the game starts or when spawned
void ADestroyableTarget::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADestroyableTarget::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}
