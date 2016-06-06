// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "RiderAnimation.generated.h"

/**
 * 
 */
UCLASS()
class BIKEV3_API URiderAnimation : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	URiderAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FootPlacement)
	FVector left_foot_position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FootPlacement)
	FVector right_foot_position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FootPlacement)
	FVector left_hand_position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FootPlacement)
	FVector right_hand_position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FootPlacement)
	FVector seat_position;
	
};
