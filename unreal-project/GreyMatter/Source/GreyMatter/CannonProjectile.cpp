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

	if (SphereVisualAsset.Succeeded()) {
			ourVisualComponent->SetStaticMesh(SphereVisualAsset.Object);
			ourVisualComponent->SetWorldScale3D(FVector(1.0f));
			UMaterial* material = LoadObjFromPath<UMaterial>("/Game/StarterContent/Materials/M_Metal_Rust.M_Metal_Rust");
			if(material) {
				ourVisualComponent->SetMaterial(0, material);
			}
	}

	ourVisualComponent->SetSimulatePhysics(true);
	ourVisualComponent->bGenerateOverlapEvents = true;
	ourVisualComponent->ComponentTags.Add(FName("__CANNON_PROJECTILE__"));

}

// Called when the game starts or when spawned
void ACannonProjectile::BeginPlay() {
	Super::BeginPlay();
	static UParticleSystem *explosionEmitter = LoadObjFromPath<UParticleSystem>("/Game/StarterContent/Particles/P_Explosion.P_Explosion");
  UGameplayStatics::SpawnEmitterAttached(explosionEmitter, RootComponent, TEXT("ExplosionEmitter1"), FVector::ZeroVector, FRotator::ZeroRotator);
	SetLifeSpan(10.0f);

	// Set the mass of the projectile
	FBodyInstance* BodyInst = ourVisualComponent->GetBodyInstance();
	BodyInst->MassScale = 200.0f;
	BodyInst->UpdateMassProperties();
}

void ACannonProjectile::ImpulseProjectileOnVector(FVector forward, float magnitude) {
	ourVisualComponent->AddImpulse(forward*magnitude, FName("None"), true);
}

// Called every frame
void ACannonProjectile::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
}
