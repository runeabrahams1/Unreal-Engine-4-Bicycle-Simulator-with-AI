// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PIDController.h"
#include "Navigation/PathFollowingComponent.h"
#include "VehiclePathFollowingComponent.generated.h"

/**
*
*/
UCLASS()
class BIKEV3_API UVehiclePathFollowingComponent : public UPathFollowingComponent
{
    GENERATED_BODY()

public:
    UVehiclePathFollowingComponent();

    UFUNCTION(BlueprintCallable, Category = "VehicleAI")
        void SetVelocityPID(float P, float I, float D, float LowerLimit, float UpperLimit);

	UFUNCTION(BlueprintCallable, Category = "VehicleAI")
		void SetDesiredSpeedInKMH(float DesiredSpeed);

	UFUNCTION(BlueprintCallable, Category = "VehicleAI")
		void SetMaxSteeringAngle(float angle);

	UFUNCTION(BlueprintCallable, Category = "VehicleAI")
		FVector GetDesiredMoveDirection();

protected:

    /** sets variables related to current move segment */
    virtual void SetMoveSegment(int32 SegmentStartIndex) override;

    /** follow current path segment */
    virtual void FollowPathSegment(float DeltaTime) override;

    /** check state of path following, update move segment if needed */
    virtual void UpdatePathSegment() override;

    /** called when owner is preparing new pathfinding request */
    virtual void OnPathfindingQuery(FPathFindingQuery& Query) override;

    /** notify about finished movement */
    virtual void OnPathFinished(EPathFollowingResult::Type Result) override;

private:

	FPIDData VelocityController;

    float CurrentSteering;
    float CurrentThrottle;
	float MaxSteeringAngle;
	float DesiredVehicleSpeedInKMH;
	FVector DesiredMoveDirection;

    float InitialDistanceToDestination;

};
