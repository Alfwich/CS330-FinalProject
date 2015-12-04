// Fill out your copyright notice in the Description page of Project Settings.

#include "GreyMatter.h"
#include "GreyMatterGameMode.h"
#include "DestroyableTarget.h"


// Sets default values
ADestroyableTarget::ADestroyableTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ourVisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetBody"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
	ourVisualComponentMaterial = LoadObjFromPath<UMaterial>("/Game/GreyMatter/Materials/Target_Mat.Target_Mat");

	if (CylinderVisualAsset.Succeeded()) {
			ourVisualComponent->SetStaticMesh(CylinderVisualAsset.Object);
			ourVisualComponent->SetWorldScale3D(FVector(1.0f));
			ourVisualComponent->SetMaterial(0, ourVisualComponentMaterial);
	}

	ourVisualComponent->SetSimulatePhysics(true);
	ourVisualComponent->SetEnableGravity(false);
	ourVisualComponent->SetNotifyRigidBodyCollision(true);
	ourVisualComponent->OnComponentHit.AddDynamic(this, &ADestroyableTarget::OnHit);

	RootComponent = ourVisualComponent;

	// Scoring
	totalHits = 1;
	scorePerHit = 10.0f;

}

void ADestroyableTarget::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if(OtherComp && OtherComp->ComponentHasTag(FName("__CANNON_PROJECTILE__"))) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Was Hit!")));
		OtherComp->ComponentTags.Remove(FName("__CANNON_PROJECTILE__"));
		AGreyMatterGameMode *gameMode = Cast<AGreyMatterGameMode>(GetWorld()->GetAuthGameMode());
		if(gameMode && totalHits > 0) {
			gameMode->alterScore(scorePerHit);
			totalHits--;
		}
	}
}


// Called when the game starts or when spawned
void ADestroyableTarget::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ADestroyableTarget::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

}
