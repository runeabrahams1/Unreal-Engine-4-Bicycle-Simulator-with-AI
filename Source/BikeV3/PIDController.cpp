// Fill out your copyright notice in the Description page of Project Settings.

#include "BikeV3.h"
#include "PIDController.h"

FPIDData::FPIDData()
    : P(1.0f)
    , I(1.0f)
    , D(1.0f)
    , LowerLimit(0.0f)
    , UpperLimit(10.0f)
    , Integral(0.0f)
    , PreviousError(0.0f)
{
}

float UPIDController::NextValue(FPIDData& PIDData, float Error, float DeltaTime)
{
    float Result;

    PIDData.Integral += Error * DeltaTime;

    float Derivative = (Error - PIDData.PreviousError) / DeltaTime;
    PIDData.PreviousError = Error;

    Result = Error      * PIDData.P +
        PIDData.Integral   * PIDData.I +
        Derivative * PIDData.D;

    return FMath::Clamp(Result, PIDData.LowerLimit, PIDData.UpperLimit);
}

void UPIDController::Clear(FPIDData& PIDData)
{
    PIDData.Integral = 0.0f;
    PIDData.PreviousError = 0.0f;
}
