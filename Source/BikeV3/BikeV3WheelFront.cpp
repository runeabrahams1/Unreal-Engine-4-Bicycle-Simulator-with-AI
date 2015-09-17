// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BikeV3.h"
#include "BikeV3WheelFront.h"
#include "Vehicles/TireType.h"

UBikeV3WheelFront::UBikeV3WheelFront()
{
	ShapeRadius = 75.0f;
	ShapeWidth = 16.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 70.0f;

	// Setup suspension forces
	SuspensionForceOffset = -5.0f;
	SuspensionMaxRaise = 10.0f;
	SuspensionMaxDrop = 10.0f;
	SuspensionNaturalFrequency = 9.0f;
	SuspensionDampingRatio = 1.05f;

	// Find the tire object and set the data for it
	static ConstructorHelpers::FObjectFinder<UTireType> TireData(TEXT("/Game/Bike/WheelData/Vehicle_FrontTireType.Vehicle_FrontTireType"));
	TireType = TireData.Object;
}
