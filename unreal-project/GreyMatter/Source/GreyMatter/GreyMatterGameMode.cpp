// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GreyMatter.h"
#include "GreyMatterGameMode.h"
#include "GreyMatterPawn.h"
#include "GreyMatterHud.h"

AGreyMatterGameMode::AGreyMatterGameMode() {
	HUDClass = AGreyMatterHud::StaticClass();
}

float AGreyMatterGameMode::getAmmoCapacity() {
	return playerAmmo;
}

void AGreyMatterGameMode::alterAmmoCapacity(float delta) {
	playerAmmo += delta;
}

float AGreyMatterGameMode::getTimeLeft() {
	return remainingTime;
}

void AGreyMatterGameMode::reset() {
	playerAmmo = 100.0f;
	remainingTime = 180;
	score = 0.0f;
	maxScore = 0.0f;
}

float AGreyMatterGameMode::getScore() {
	return score;
}

void AGreyMatterGameMode::addToMaxScore(float delta) {
	maxScore += delta;
}

void AGreyMatterGameMode::alterScore(float delta) {
	score += delta;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("New Score: %f/%f"), score, maxScore));
}

void AGreyMatterGameMode::BeginPlay() {
	Super::BeginPlay();
	if(!GetWorld()->GetMapName().Contains("MenuMap")) {
		reset();
		GetWorldTimerManager().SetTimer(this, &AGreyMatterGameMode::tickRemaining, 1.0f, true);
	}
}

bool AGreyMatterGameMode::gameIsOver() {
	return score == maxScore || playerAmmo <= 0.0f;
}

void AGreyMatterGameMode::tickRemaining() {
	remainingTime -= 1;

	if(remainingTime > 0) {
		if(gameIsOver()) {
			remainingTime = 0;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Round over with total score of: %f, using %f ammo, for efficency:%f"), score, 100.0f-playerAmmo, score/(100.0f-playerAmmo)));
		} else if(((int32)remainingTime)%5 == 0) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Time Remaining: %d"), remainingTime));
		}
	} else if(remainingTime == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Round over with total score of: %f, using %f ammo, for efficency:%f"), score, 100.0f-playerAmmo, score/(100.0f-playerAmmo)));
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Returning to main menu in: %d"), 5+remainingTime));
	}


	if(remainingTime <= -5) {
		UGameplayStatics::OpenLevel(GetWorld(), FName("MenuMap"));
	}

}
