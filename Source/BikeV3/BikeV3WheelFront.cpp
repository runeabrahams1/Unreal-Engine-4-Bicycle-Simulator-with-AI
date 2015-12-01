// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BikeV3.h"
#include "BikeV3WheelFront.h"
#include "Vehicles/TireType.h"

UBikeV3WheelFront::UBikeV3WheelFront()
{
	ShapeRadius = 36.5f;
	ShapeWidth = 10.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 70.0f;

	// Setup suspension forces
	SuspensionForceOffset = -2.5f;
	SuspensionMaxRaise = 0.0f;
	SuspensionMaxDrop = 20.0f;
	SuspensionNaturalFrequency = 36.0f;
	SuspensionDampingRatio = 10.05f;

	// Find the tire object and set the data for it
	static ConstructorHelpers::FObjectFinder<UTireType> TireData(TEXT("/Game/Bicycle/WheelData/BicycleTire.BicycleTire"));
	TireType = TireData.Object;
	TireType->SetFrictionScale(15.f);
}
