// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GreyMatter.h"
#include "GreyMatterHud.h"
#include "GreyMatterPawn.h"
#include "GreyMatterGameMode.h"
#include "GameFramework/WheeledVehicle.h"
#include "RenderResource.h"
#include "Shader.h"
#include "Engine/Canvas.h"
#include "Vehicles/WheeledVehicleMovementComponent.h"
#include "Engine/Font.h"
#include "CanvasItem.h"
#include "Engine.h"

// Needed for VR Headset
#ifdef HMD_INTGERATION
#include "IHeadMountedDisplay.h"
#endif // HMD_INTGERATION


#define LOCTEXT_NAMESPACE "VehicleHUD"

AGreyMatterHud::AGreyMatterHud()
{
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = Font.Object;
}

void AGreyMatterHud::DrawHUD()
{
	Super::DrawHUD();

	// Calculate ratio from 720p
	const float HUDXRatio = Canvas->SizeX / 1280.f;
	const float HUDYRatio = Canvas->SizeY / 720.f;

	bool bWantHUD = true;
#ifdef HMD_INTGERATION
	if (GEngine->HMDDevice.IsValid() == true)
	{
		bWantHUD = GEngine->HMDDevice->IsStereoEnabled();
	}
#endif// HMD_INTGERATION
	// We dont want the onscreen hud when using a HMD device	
	if (bWantHUD == true)
	{
		// Get our vehicle so we can check if we are in car. If we are we don't want onscreen HUD
		AGreyMatterPawn* Vehicle = Cast<AGreyMatterPawn>(GetOwningPawn());
		if ((Vehicle != nullptr) && (Vehicle->bInCarCameraActive == false))
		{
			FVector2D ScaleVec(HUDYRatio * 1.4f, HUDYRatio * 1.4f);

			// Speed
			FCanvasTextItem SpeedTextItem(FVector2D(HUDXRatio * 805.f, HUDYRatio * 455), Vehicle->SpeedDisplayString, HUDFont, FLinearColor::White);
			SpeedTextItem.Scale = ScaleVec;
			Canvas->DrawItem(SpeedTextItem);

			// Gear
			FCanvasTextItem GearTextItem(FVector2D(HUDXRatio * 805.f, HUDYRatio * 500.f), Vehicle->GearDisplayString, HUDFont, Vehicle->bInReverseGear == false ? Vehicle->GearDisplayColor : Vehicle->GearDisplayReverseColor);
            
            
			GearTextItem.Scale = ScaleVec;
			Canvas->DrawItem(GearTextItem);
            
            // BEGIN MY CODE
/*
            AGreyMatterGameMode *gameMode = Cast<AGreyMatterGameMode>(GetWorld()->GetAuthGameMode());
            
            FCanvasTextItem AmmoItem(FVector2D(HUDXRatio * 805.f, HUDYRatio * 545.f), gameMode->getAmmoCapacity(), HUDFont, FLinearColor::Red);
            AmmoItem.Scale = ScaleVec;
            Canvas->DrawItem(AmmoItem);
            
            FCanvasTextItem TimeLeftItem(FVector2D(HUDXRatio * 200.f, HUDYRatio * 100.f), gameMode->getTimeLeft(), HUDFont, FLinearColor::White);
            TimeLeftItem.Scale = ScaleVec;
            Canvas->DrawItem(FCanvasTextItem);
*/
            // END MY CODE
            

		}
	}
}


#undef LOCTEXT_NAMESPACE
