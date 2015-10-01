// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Vehicles/WheeledVehicleMovementComponent.h"
#include "BikeMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class BIKEV3_API UBikeMovementComponent : public UWheeledVehicleMovementComponent
{
	GENERATED_UCLASS_BODY()
	
	/** Maximum steering versus forward speed (km/h) */
	UPROPERTY(EditAnywhere, Category = SteeringSetup)
	FRuntimeFloatCurve SteeringCurve;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
