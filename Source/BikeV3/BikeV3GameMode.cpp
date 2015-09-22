// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BikeV3.h"
#include "BikeV3GameMode.h"
#include "CollidingPawn.h"
#include "BikePlayerController.h"
#include "BikeV3Hud.h"

ABikeV3GameMode::ABikeV3GameMode()
{
	DefaultPawnClass = ACollidingPawn::StaticClass();
	HUDClass = ABikeV3Hud::StaticClass();
	PlayerControllerClass = ABikePlayerController::StaticClass();
}
