// Fill out your copyright notice in the Description page of Project Settings.

#include "BikeV3.h"
#include "SerialClass.h"
#include "Engine.h"
#include "CollidingPawnMovementComponent.h"
#include <stdlib.h>

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

void UCollidingPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

};

int readArduino()
{
	SerialClass* SP = new SerialClass("\\\\.\\COM3");    // adjust as needed
	char incomingData[256] = "";			// don't forget to pre-allocate memory
	//printf("%s\n",incomingData);
	int dataLength = 256;
	int readResult = 0;
	if (SP->IsConnected())
	{
		readResult = SP->ReadData(incomingData, dataLength);
	}
	int a = atoi(incomingData);
	return a;
}



