// Fill out your copyright notice in the Description page of Project Settings.

#include "BikeV3.h"
#include "RiderAnimation.h"

URiderAnimation::URiderAnimation()
{
	URiderAnimation::left_foot_position = FVector(0.f, 0.f, 0.f);
	URiderAnimation::right_foot_position = FVector(0.f, 0.f, 0.f);
	URiderAnimation::left_hand_position = FVector(0.f, 0.f, 0.f);
	URiderAnimation::right_hand_position = FVector(0.f, 0.f, 0.f);
	URiderAnimation::seat_position = FVector(0.f, 0.f, 0.f);

}


