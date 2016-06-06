// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BikeV3.h"
#include "BikeV3WheelRear.h"
#include "Vehicles/TireType.h"

UBikeV3WheelRear::UBikeV3WheelRear()
{
	ShapeRadius = 37.5f;
	ShapeWidth = 10.0f;
	bAffectedByHandbrake = true;
	SteerAngle = 0.0f;
	DampingRate = 1.05f;

	LatStiffMaxLoad = 5.0f;
	//LatStiffValue = 10.0f;
	//LongStiffValue = 1000.0f;

	// Setup suspension forces

	SuspensionMaxRaise = 1.0f;
	SuspensionMaxDrop = 2.0f;
	SuspensionNaturalFrequency = 0.90f;
	SuspensionDampingRatio = 1.05f;

	// Find the tire object and set the data for it
	static ConstructorHelpers::FObjectFinder<UTireType> TireData(TEXT("/Game/Bicycle/WheelData/BicycleTire.BicycleTire"));
	TireType = TireData.Object;
}
