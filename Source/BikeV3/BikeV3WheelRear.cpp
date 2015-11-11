// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BikeV3.h"
#include "BikeV3WheelRear.h"
#include "Vehicles/TireType.h"

UBikeV3WheelRear::UBikeV3WheelRear()
{
	ShapeRadius = 36.5f;
	ShapeWidth = 9.0f;
	bAffectedByHandbrake = true;
	SteerAngle = 0.0f;

	// Setup suspension forces
	SuspensionForceOffset = 0.0f;
	SuspensionMaxRaise = 0.0f;
	SuspensionMaxDrop = 0.0f;
	SuspensionNaturalFrequency = 9.0f;
	SuspensionDampingRatio = 1.05f;

	// Find the tire object and set the data for it
	static ConstructorHelpers::FObjectFinder<UTireType> TireData(TEXT("/Game/Bicycle/WheelData/BicycleTire.BicycleTire"));
	TireType = TireData.Object;
}
