// Fill out your copyright notice in the Description page of Project Settings.

#include "BikeV3.h"
#include "BikeIgnoreWheel.h"
#include "Vehicles/TireType.h"


UBikeIgnoreWheel::UBikeIgnoreWheel()
{
	ShapeRadius = 0.0f;
	ShapeWidth = 0.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 0.0f;

	// Setup suspension forces
	SuspensionForceOffset = -4.0f;
	SuspensionMaxRaise = 10.0f;
	SuspensionMaxDrop = 10.0f;
	SuspensionNaturalFrequency = 9.0f;
	SuspensionDampingRatio = 1.05f;

	// Find the tire object and set the data for it
	static ConstructorHelpers::FObjectFinder<UTireType> TireData(TEXT("/Game/Bike/WheelData/Ignore_This_Wheel.Ignore_This_Wheel"));
	TireType = TireData.Object;
}



