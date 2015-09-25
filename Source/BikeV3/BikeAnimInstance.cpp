// Fill out your copyright notice in the Description page of Project Settings.

#include "BikeV3.h"
#include "BikeAnimInstance.h"

UBikeAnimInstance::UBikeAnimInstance()
{
	//set any default values for your variables here
	SkelControl_HandleRot = FRotator(0, 0, 0);
	SkelControl_WheelRot_F = FRotator(0, 0, 0);
	SkelControl_WheelRot_B = FRotator(0, 0, 0);
}



