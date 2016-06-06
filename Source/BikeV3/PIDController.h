// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PIDController.generated.h"

/**
* Used to implement closed-loop control on variables which depend on time.
* https://en.wikipedia.org/wiki/PID_controller
*/
USTRUCT(BlueprintType)
struct BIKEV3_API FPIDData
{

    GENERATED_USTRUCT_BODY()

    friend class UPIDController;

public:

    FPIDData();

    UPROPERTY(EditAnywhere, Category = "PID")
        float P;

    UPROPERTY(EditAnywhere, Category = "PID")
        float I;

    UPROPERTY(EditAnywhere, Category = "PID")
        float D;

    UPROPERTY(EditAnywhere, Category = "PID")
        float UpperLimit;

    UPROPERTY(EditAnywhere, Category = "PID")
        float LowerLimit;

private:

    float Integral;
    float PreviousError;
};

UCLASS()
class BIKEV3_API UPIDController : public UObject
{
    GENERATED_BODY()

public:
    /**
        Calculates the next result using the (P)roportional,
        (I)ntegral, and (D)erivative gain terms using the error
        and the time since the last measurement was taken.

        Error is defined as the desired varaible value minus the
        sensed variable value. E.g., Desired vehicle speed is 40km/h
        minus sensed vehicle speed of 20km/h.

        \param Error Difference between desired and sensed values
        \param DeltaTime Time since last measurement
        \return the Calculated PID value
    */
    UFUNCTION(BlueprintCallable, Category = "PID")
        static float NextValue(FPIDData& PIDData, float Error, float DeltaTime);

    /**
        Resets the integral and derivative related variables.
    */
    UFUNCTION(BlueprintCallable, Category = "PID")
        static void Clear(FPIDData& PIDData);
};

