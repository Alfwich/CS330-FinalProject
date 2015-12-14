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

void AGreyMatterHud::DrawHUD() {
	Super::DrawHUD();

	// Calculate ratio from 720p
	const float HUDXRatio = Canvas->SizeX / 1280.f;
	const float HUDYRatio = Canvas->SizeY / 720.f;

	// Get our vehicle so we can check if we are in car. If we are we don't want onscreen HUD
	FVector2D ScaleVec(HUDYRatio * 1.4f, HUDYRatio * 1.4f);

  AGreyMatterGameMode *gameMode = Cast<AGreyMatterGameMode>(GetWorld()->GetAuthGameMode());
	if(gameMode->gameIsOver()) {
			{
				FText endText = FText::Format( LOCTEXT("EndText1", "Round over with score {0}"), FText::AsNumber(gameMode->getScore()));
				FCanvasTextItem endTextItem(FVector2D(HUDXRatio * (Canvas->SizeY/2.0f), HUDYRatio * 200.0f), endText, HUDFont, FLinearColor::White);
				endTextItem.Scale = ScaleVec;
				Canvas->DrawItem(endTextItem);
			}

			{
				int32 timeRemaining = gameMode->getTimeLeft();
				if(timeRemaining > 0) {
					timeRemaining = 0;
				}
				FText endText = FText::Format( LOCTEXT("EndText2", "Returning to main menu in {0}"), FText::AsNumber(5.0f + timeRemaining));
				FCanvasTextItem endTextItem(FVector2D(HUDXRatio * (Canvas->SizeY/2.0f), HUDYRatio * 245.0f), endText, HUDFont, FLinearColor::White);
				endTextItem.Scale = ScaleVec;
				Canvas->DrawItem(endTextItem);
			}
	} else {
		FCanvasTextItem ScoreTextItem(FVector2D(HUDXRatio * 805.0f, HUDYRatio * 455.0f), FText::Format(LOCTEXT("ScoreFormat", "Score: {0}"), FText::AsNumber(gameMode->getScore())), HUDFont, FLinearColor::White);
		ScoreTextItem.Scale = ScaleVec;
		Canvas->DrawItem(ScoreTextItem);

		int32 ammoLeft = gameMode->getAmmoCapacity();
		FLinearColor ammoColor = FLinearColor(1.0f, ammoLeft/100.0f, ammoLeft/100.0f, 1.0f);//(ammoLeft > 0) ? FLinearColor::White : FLinearColor::Red;
		FCanvasTextItem AmmoTextItem(FVector2D(HUDXRatio * 805.0f, HUDYRatio * 500.0f), FText::Format(LOCTEXT("AmmoFormat", "Ammo: {0}"), FText::AsNumber(gameMode->getAmmoCapacity())), HUDFont, ammoColor);
		AmmoTextItem.Scale = ScaleVec;
		Canvas->DrawItem(AmmoTextItem);

		int32 timeLeft = gameMode->getTimeLeft();
		FLinearColor timeColor = FLinearColor(1.0f, timeLeft/180.0f, timeLeft/180.0f, 1.0f);//(ammoLeft > 0) ? FLinearColor::White : FLinearColor::Red;
		FCanvasTextItem TimeLeftItem(FVector2D(HUDXRatio * 805.0f, HUDYRatio * 545.0f), FText::Format(LOCTEXT("TimeLeftFormat", "Time Left: {0}"), FText::AsNumber(timeLeft)), HUDFont, timeColor);
		TimeLeftItem.Scale = ScaleVec;
		Canvas->DrawItem(TimeLeftItem);

		if(timeLeft >= 175) {
			FText infoText = LOCTEXT("InfoText", "Shoot all the targets to win!");
			FCanvasTextItem InfoTextItem(FVector2D(HUDXRatio * (Canvas->SizeY/2.0f), HUDYRatio * 200.0f), infoText, HUDFont, FLinearColor::White);
			InfoTextItem.Scale = ScaleVec;
			Canvas->DrawItem(InfoTextItem);

		}

	}


}


#undef LOCTEXT_NAMESPACE
