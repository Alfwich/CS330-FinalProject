// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GreyMatter.h"
#include "GreyMatterGameMode.h"
#include "GreyMatterPawn.h"
#include "GreyMatterWheelFront.h"
#include "GreyMatterWheelRear.h"
#include "GreyMatterHud.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Vehicles/WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine.h"
#include "CannonProjectile.h"

#ifdef HMD_INTGERATION
// Needed for VR Headset
#include "IHeadMountedDisplay.h"
#endif // HMD_INTGERATION

const FName AGreyMatterPawn::LookUpBinding("LookUp");
const FName AGreyMatterPawn::LookRightBinding("LookRight");

#define LOCTEXT_NAMESPACE "VehiclePawn"

AGreyMatterPawn::AGreyMatterPawn() {
	// Car mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicle/Sedan/Sedan_SkelMesh.Sedan_SkelMesh"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);

	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicle/Sedan/Sedan_AnimBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

	// Simulation
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = UGreyMatterWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UGreyMatterWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = UGreyMatterWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = UGreyMatterWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->AttachTo(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	// Create camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	// Create text render component for in car speed display
	InCarSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarSpeed"));
	InCarSpeed->SetRelativeLocation(FVector(70.0f, -75.0f, 99.0f));
	InCarSpeed->SetRelativeRotation(FRotator(18.0f, 180.0f, 0.0f));
	InCarSpeed->AttachTo(GetMesh());
	InCarSpeed->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));

	// Create text render component for in car gear display
	InCarGear = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
	InCarGear->SetRelativeLocation(FVector(66.0f, -9.0f, 95.0f));
	InCarGear->SetRelativeRotation(FRotator(25.0f, 180.0f,0.0f));
	InCarGear->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));
	InCarGear->AttachTo(GetMesh());

	// Colors for the incar gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	// Colors for the in-car gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	bInReverseGear = false;
	controlsAreEnabled = true;

	turretBaseCmp = NULL;
	turretBarrelFacingCmp = NULL;

	cannonPitchOffset = 25.0f;
}

void AGreyMatterPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AGreyMatterPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AGreyMatterPawn::MoveRight);
	InputComponent->BindAxis("LookUp");
	InputComponent->BindAxis("LookRight");

	InputComponent->BindAction("ShootCannon", IE_Pressed, this, &AGreyMatterPawn::OnShootCannon);
}

void AGreyMatterPawn::MoveForward(float Val) {
	if(controlsAreEnabled) {
		GetVehicleMovementComponent()->SetThrottleInput(Val);
	}
}

void AGreyMatterPawn::MoveRight(float Val) {
	if(controlsAreEnabled) {
		GetVehicleMovementComponent()->SetSteeringInput(Val);
	}
}

void AGreyMatterPawn::OnShootCannon() {
	if(controlsAreEnabled) {
		UWorld* const World = GetWorld();
		if(World && turretBarrelFacingCmp) {
			AGreyMatterGameMode *gameMode = Cast<AGreyMatterGameMode>(World->GetAuthGameMode());
			if(gameMode->getAmmoCapacity() > 0.0f) {
				gameMode->alterAmmoCapacity(-1.0);
				FVector projectileLocation = turretBarrelFacingCmp->GetComponentLocation();
				ACannonProjectile* projectile = World->SpawnActor<ACannonProjectile>(projectileLocation, FRotator::ZeroRotator);

				FVector projectileDirection = turretBarrelFacingCmp->GetForwardVector();
				projectile->ImpulseProjectileOnVector(projectileDirection, 5000.0f + GetVehicleMovement()->GetForwardSpeed());
			}
		}
	}
}

void AGreyMatterPawn::OnHandbrakePressed() {
}

void AGreyMatterPawn::OnHandbrakeReleased() {
}

void AGreyMatterPawn::OnToggleCamera() {
}

void AGreyMatterPawn::EnableIncarView(const bool bState, const bool bForce) {
}


void AGreyMatterPawn::Tick(float Delta) {
	AGreyMatterGameMode *gameMode = Cast<AGreyMatterGameMode>(GetWorld()->GetAuthGameMode());
	if(gameMode->getTimeLeft() > 0) {
		// Setup the flag to say we are in reverse gear
		bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;

		// Update the strings used in the hud (incar and onscreen)
		UpdateHUDStrings();

		// Set the string in the incar hud
		SetupInCarHUD();

		// Update the behind camera rotation
	  FRotator HeadRotation = SpringArm->RelativeRotation;
	  HeadRotation.Pitch += InputComponent->GetAxisValue(LookUpBinding);
	  HeadRotation.Yaw += InputComponent->GetAxisValue(LookRightBinding);
		SpringArm->RelativeRotation = HeadRotation;
		SetCannonRotation(HeadRotation);
	} else if(controlsAreEnabled) {
		controlsAreEnabled = false;
		GetVehicleMovementComponent()->SetThrottleInput(0.0f);
		GetVehicleMovementComponent()->SetSteeringInput(0.0f);
	}
	SpringArm->TargetArmLength = FMath::Clamp( 600.0f + (GetVehicleMovement()->GetForwardSpeed()/6.0f), 600.0f, 2000.0f);
}

void AGreyMatterPawn::SetCannonRotation(FRotator rotation) {
	if(turretBaseCmp) {
		rotation.Pitch += cannonPitchOffset;
		turretBaseCmp->RelativeRotation = rotation;
	}
}

void AGreyMatterPawn::SetupCannonReferences() {
		const TArray<UActorComponent*> comps = GetComponents();
		for(int i = 0; i < comps.Num(); ++i) {
			UActorComponent *cmp = comps[i];
			if(cmp->ComponentHasTag(FName("CannonRoot"))) {
				turretBaseCmp = Cast<UStaticMeshComponent>(cmp);
				continue;
			}

			if(cmp->ComponentHasTag(FName("CannonProjectileStart"))) {
				turretBarrelFacingCmp = Cast<UArrowComponent>(cmp);
				continue;
			}
		}
}

void AGreyMatterPawn::BeginPlay() {
	SetupCannonReferences();
}

void AGreyMatterPawn::OnResetVR() {
}

void AGreyMatterPawn::UpdateHUDStrings() {
	float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	int32 KPH_int = FMath::FloorToInt(KPH);

	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));

	if (bInReverseGear == true)
	{
		GearDisplayString = FText(LOCTEXT("ReverseGear", "R"));
	}
	else
	{
		int32 Gear = GetVehicleMovement()->GetCurrentGear();
		GearDisplayString = (Gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(Gear);
	}
}

void AGreyMatterPawn::SetupInCarHUD() {
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ((PlayerController != nullptr) && (InCarSpeed != nullptr) && (InCarGear != nullptr) )
	{
		// Setup the text render component strings
		InCarSpeed->SetText(SpeedDisplayString);
		InCarGear->SetText(GearDisplayString);

		if (bInReverseGear == false)
		{
			InCarGear->SetTextRenderColor(GearDisplayColor);
		}
		else
		{
			InCarGear->SetTextRenderColor(GearDisplayReverseColor);
		}
	}
}

#undef LOCTEXT_NAMESPACE
