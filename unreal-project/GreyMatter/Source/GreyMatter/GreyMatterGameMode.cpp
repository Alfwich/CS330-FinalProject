// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GreyMatter.h"
#include "GreyMatterGameMode.h"
#include "GreyMatterPawn.h"
#include "GreyMatterHud.h"

AGreyMatterGameMode::AGreyMatterGameMode()
{
	DefaultPawnClass = AGreyMatterPawn::StaticClass();
	HUDClass = AGreyMatterHud::StaticClass();
}
