// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE4DuinoPCH.h"
#include "SerialClass.h"
#include "ThreadBP.generated.h"

UCLASS(Blueprintable)
class UThreadBP : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
private:

	static CSerial *SP;
	static UThreadBP* BP;
	

public:

	UThreadBP(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "UE4Duino")
		static void OpenSerial(int32 portNum, int32 baudRate);

	UFUNCTION(BlueprintCallable, Category = "UE4Duino")
		static void CloseSerial();

	UFUNCTION(BlueprintCallable, Category = "UE4Duino")
		static FString ReadSerial();

	UFUNCTION(BlueprintCallable, Category = "UE4Duino")
		static void WriteSerial(FString writeData);

	UFUNCTION(BlueprintCallable, Category = "UE4Duino")
		static bool IsSerialPortOpen();


	void StartThread(int32 portNum, int32 baudRate);
	FString GetData(FString data);
	FString SendData();
	void IsThreadClosed(bool threadClosed);
	void IsSerialClosed(bool serialClosed);
	void SerialOpened(bool serialOpen);



	/*UFUNCTION(BlueprintCallable, Category = "UE4Duino")
		static FString TestWriteRuturn();

	UFUNCTION(BlueprintCallable, Category = "UE4Duino")
		static void GetWriteRuturn(FString testReturn);*/
};
	
