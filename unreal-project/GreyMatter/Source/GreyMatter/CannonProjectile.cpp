// Fill out your copyright notice in the Description page of Project Settings.

#include "GreyMatter.h"
#include "CannonProjectile.h"


// Sets default values
ACannonProjectile::ACannonProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create and position a mesh component so we can see where our sphere is
	ourVisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletRepresentation"));
	RootComponent = ourVisualComponent;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	UMaterial* material = LoadObjFromPath<UMaterial>("/Game/StarterContent/Materials/M_Metal_Rust.M_Metal_Rust");

	if (SphereVisualAsset.Succeeded()) {
			ourVisualComponent->SetStaticMesh(SphereVisualAsset.Object);
			ourVisualComponent->SetWorldScale3D(FVector(0.2f));
			ourVisualComponent->SetMaterial(0, material);
	}

	ourVisualComponent->SetSimulatePhysics(true);

}

// Called when the game starts or when spawned
void ACannonProjectile::BeginPlay() {
	Super::BeginPlay();
	static UParticleSystem *explosionEmitter = LoadObjFromPath<UParticleSystem>("/Game/StarterContent/Particles/P_Explosion.P_Explosion");
  UGameplayStatics::SpawnEmitterAttached(explosionEmitter, RootComponent, TEXT("ExplosionEmitter1"), FVector::ZeroVector, FRotator::ZeroRotator);
	SetLifeSpan(10.0f);
}

void ACannonProjectile::ImpulseProjectileOnVector(FVector forward, float magnitude) {
	ourVisualComponent->AddImpulse(forward*magnitude, FName("None"), true);
}

// Called every frame
void ACannonProjectile::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
}
