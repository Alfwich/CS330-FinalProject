// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "GreyMatterGameMode.generated.h"


UCLASS(minimalapi)
class AGreyMatterGameMode : public AGameMode
{
	GENERATED_BODY()

	float playerAmmo;
	float remainingTime;
	float score;

public:
	AGreyMatterGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Ammo
	float getAmmoCapacity();
	void alterAmmoCapacity(float delta);

	// Time
	float getTimeLeft();

	// Score
	float getScore();
	void alterScore(float delta);

	void tickRemaining();
	void reset();


};
