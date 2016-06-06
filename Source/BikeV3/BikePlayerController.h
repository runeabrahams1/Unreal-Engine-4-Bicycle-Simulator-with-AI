// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "BikePlayerController.generated.h"

/**
 * 
 */
UCLASS(config=Game)
class BIKEV3_API ABikePlayerController : public APlayerController
{
	GENERATED_BODY()

	ABikePlayerController();
	
	virtual void SetupInputComponent() override;

};
