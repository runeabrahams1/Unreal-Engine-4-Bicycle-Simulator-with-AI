// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "CollidingPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class BIKEV3_API UCollidingPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
private:

	int readArduino();
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	bool pressedBrakeLeft();
	bool pressedBrake
	
};
