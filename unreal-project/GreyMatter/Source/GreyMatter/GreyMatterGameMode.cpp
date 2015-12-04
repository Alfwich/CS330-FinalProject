// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GreyMatter.h"
#include "GreyMatterGameMode.h"
#include "GreyMatterPawn.h"
#include "GreyMatterHud.h"

AGreyMatterGameMode::AGreyMatterGameMode() {
	DefaultPawnClass = AGreyMatterPawn::StaticClass();
	HUDClass = AGreyMatterHud::StaticClass();
	reset();
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
	remainingTime = 30.0f;
	score = 0.0f;
}

float AGreyMatterGameMode::getScore() {
	return score;
}

void AGreyMatterGameMode::alterScore(float delta) {
	score += delta;
}

void AGreyMatterGameMode::BeginPlay() {
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(this, &AGreyMatterGameMode::tickRemaining, 1.0f, true);
}

void AGreyMatterGameMode::tickRemaining() {
	if(remainingTime) {
		remainingTime -= 1.0f;

		if(remainingTime == 0.0f) {
			// Do level cleanup
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Time Remaining: %f"), remainingTime));
}
