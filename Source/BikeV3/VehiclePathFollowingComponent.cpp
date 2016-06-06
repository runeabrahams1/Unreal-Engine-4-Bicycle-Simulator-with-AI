// Fill out your copyright notice in the Description page of Project Settings.

#include "BikeV3.h"
#include "VehiclePathFollowingComponent.h"
#include "GameFramework/WheeledVehicle.h"
#include "Vehicles/WheeledVehicleMovementComponent.h"
#include "BikeV3Pawn.h"

UVehiclePathFollowingComponent::UVehiclePathFollowingComponent()
	: CurrentThrottle(0.0f)
	, CurrentSteering(0.0f)
	, InitialDistanceToDestination(-1.0f)
	, MaxSteeringAngle(5.0f)
	, DesiredVehicleSpeedInKMH(0.0f)
{
    VelocityController.P = 0.05f;
    VelocityController.I = 0.001f;
    VelocityController.D = 0.03f;
    VelocityController.LowerLimit = -1.0f;
    VelocityController.UpperLimit = 1.0f;
}

void UVehiclePathFollowingComponent::SetVelocityPID(float P, float I, float D, float LowerLimit, float UpperLimit)
{
    VelocityController.P = P;
    VelocityController.I = I;
    VelocityController.D = D;
    VelocityController.LowerLimit = LowerLimit;
    VelocityController.UpperLimit = UpperLimit;
}

void UVehiclePathFollowingComponent::SetMaxSteeringAngle(float angle)
{
	MaxSteeringAngle = angle;
}

void UVehiclePathFollowingComponent::SetDesiredSpeedInKMH(float desiredSpeed)
{
	DesiredVehicleSpeedInKMH = desiredSpeed;
}

void UVehiclePathFollowingComponent::SetMoveSegment(int32 SegmentStartIndex)
{
    UPathFollowingComponent::SetMoveSegment(SegmentStartIndex);

    AWheeledVehicle* Owner = Cast<AWheeledVehicle>(MovementComp->GetOwner());

    if (Owner)
    {
        InitialDistanceToDestination = (Owner->GetActorLocation() - GetCurrentTargetLocation()).Size();
    }
}

FVector UVehiclePathFollowingComponent::GetDesiredMoveDirection()
{
	return DesiredMoveDirection;
}

void UVehiclePathFollowingComponent::FollowPathSegment(float DeltaTime)
{
    if (MovementComp)
    {
        UWheeledVehicleMovementComponent* VehicleMoveComp =
            Cast<UWheeledVehicleMovementComponent>(MovementComp);

        AWheeledVehicle* Owner = Cast<AWheeledVehicle>(VehicleMoveComp->GetOwner());

        if (Owner && VehicleMoveComp)
        {
			if(DesiredVehicleSpeedInKMH == 0.0f)
				VehicleMoveComp->SetHandbrakeInput(true);
			else
				VehicleMoveComp->SetHandbrakeInput(false);

            FVector VehicleLocation = Owner->GetActorLocation();
            FVector Destination = GetCurrentTargetLocation();
            FVector DirectionToDestination = Destination - VehicleLocation; //Vector pointing to current destination (not necessarily the final destination)

			//____STEERING____
			float DesiredSteering = 0.f;
			float DeltaYaw = (DirectionToDestination.ToOrientationRotator() - Owner->GetActorForwardVector().ToOrientationRotator()).Yaw;
            bool DestinationToRight = DeltaYaw >= 0;

			if (DeltaYaw > 180.f || DeltaYaw < -180.f) {
				DestinationToRight = !DestinationToRight;
				if (DestinationToRight && DeltaYaw >= -360.f + MaxSteeringAngle)
				{
					DesiredSteering = 1.f;
				}
				else if (!DestinationToRight && DeltaYaw <= 360.f - MaxSteeringAngle)
				{
					DesiredSteering = -1.f;
				}
				else if(DestinationToRight)
				{
					DesiredSteering = FMath::GetMappedRangeValueClamped(FVector2D(-360.f, -360.f + MaxSteeringAngle), FVector2D(0, 1.0f), DeltaYaw);
				}
				else
				{
					DesiredSteering = FMath::GetMappedRangeValueClamped(FVector2D(360.f - MaxSteeringAngle, 360.f), FVector2D(-1.f, 0.f), DeltaYaw);
				}
			}
			else
			{
				if (DestinationToRight && DeltaYaw >= MaxSteeringAngle)
				{
					DesiredSteering = 1.f;
				}
				else if (!DestinationToRight && DeltaYaw <= -MaxSteeringAngle)
				{
					DesiredSteering = -1.f;
				}
				else
					DesiredSteering = FMath::GetMappedRangeValueClamped(FVector2D(-MaxSteeringAngle, MaxSteeringAngle), FVector2D(-1.f, 1.f), DeltaYaw);
			}

			CurrentSteering = DesiredSteering;
            VehicleMoveComp->SetSteeringInput(CurrentSteering);
			//____END STEERING____

			//____THROTTLE____
			float CurrentSpeedInKMH = VehicleMoveComp->GetForwardSpeed() * 0.036f; //Convert to KM/H
			//TODO: Allow speed to be 0, but never 0 if DesiredSpeed is > 0
			float EstiamtedDesiredSpeed = DesiredVehicleSpeedInKMH - (FMath::Abs(CurrentSteering) * DesiredVehicleSpeedInKMH * 0.8);

            CurrentThrottle += UPIDController::NextValue(VelocityController, EstiamtedDesiredSpeed - CurrentSpeedInKMH, DeltaTime);
			CurrentThrottle = FMath::Clamp(CurrentThrottle, 0.f, 1.f);
            VehicleMoveComp->SetThrottleInput(CurrentThrottle);
			//____END THROTTLE____

			//Direction the wheel is facing
			DesiredMoveDirection = Owner->GetActorForwardVector().ToOrientationRotator().Add(0.f, FMath::GetMappedRangeValueClamped(FVector2D(-1.f, 1.f), FVector2D(-90.f, 90.f), CurrentSteering), 0.f).Vector();
			ABikeV3Pawn* BikePawn = Cast<ABikeV3Pawn>(Owner);
			if(BikePawn)
				BikePawn->CurrentHandlingInput = CurrentSteering;

#ifdef NDEBUG
			float DistanceToDestination = DirectionToDestination.Size();
            DrawDebugPoint(GetWorld(), Destination, 50.0f, FColor::Blue);
            DrawDebugLine(GetWorld(), VehicleLocation, VehicleLocation + DirectionToDestination, FColor::Yellow);

            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Cyan, FString::Printf(TEXT("Throttle %0.3f Steering: %0.3f"), CurrentThrottle, CurrentSteering));
                GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Cyan, FString::Printf(TEXT("Distance to destination: %0.2f"), DistanceToDestination));
				GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Cyan, FString::Printf(TEXT("Current Speed in KM/H: %0.2f"), CurrentSpeedInKMH));
				GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Cyan, FString::Printf(TEXT("DeltaYaw %0.3f"), DeltaYaw));
            }
#endif

        }
    }
}

void UVehiclePathFollowingComponent::UpdatePathSegment()
{
#ifdef NDEBUG
	if (Path.IsValid())
	{
		for (int i = 0; i < Path->GetPathPoints().Num(); ++i)
		{
			DrawDebugPoint(GetWorld(), Path->GetPathPoints()[i].Location, 40.0f, FColor::Yellow, false, 0.3f);
		}
	}
#endif

    float DistanceLeft = (
        *Path->GetPathPointLocation(Path->GetPathPoints().Num() - 1)
        - MovementComp->GetActorLocation()
    ).Size();

    if (DistanceLeft < AcceptanceRadius)
    {
        OnSegmentFinished();
        OnPathFinished(EPathFollowingResult::Success);
    }
	UPathFollowingComponent::UpdatePathSegment();
}

void UVehiclePathFollowingComponent::OnPathfindingQuery(FPathFindingQuery& Query)
{
#ifdef NDEBUG
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(4, 3, FColor::Blue, TEXT("Custom Path finding queried"));
    }
#endif

    //TODO: Perform avoidance checks here. Or just add NavMeshModifiers/CollisionBoxes around actors

	UPathFollowingComponent::OnPathfindingQuery(Query);
}

void UVehiclePathFollowingComponent::OnPathFinished(EPathFollowingResult::Type Result)
{
#ifdef NDEBUG
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Green, "Path Following Result: " + FString::FromInt(Result));
    }
#endif

	UWheeledVehicleMovementComponent* VehicleMoveComp =
		Cast<UWheeledVehicleMovementComponent>(MovementComp);
	
    InitialDistanceToDestination = -1.0f;
	if (VehicleMoveComp) {
		VehicleMoveComp->SetThrottleInput(0.0f);
		VehicleMoveComp->SetSteeringInput(0.0f);
		VehicleMoveComp->SetHandbrakeInput(true);
	}

    UPIDController::Clear(VelocityController);
    UPathFollowingComponent::OnPathFinished(Result);
}
