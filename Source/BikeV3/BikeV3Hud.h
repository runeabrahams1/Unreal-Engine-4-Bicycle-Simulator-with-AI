// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "BikeV3Hud.generated.h"


UCLASS(config = Game)
class ABikeV3Hud : public AHUD
{
	GENERATED_BODY()

public:
	ABikeV3Hud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface

};
