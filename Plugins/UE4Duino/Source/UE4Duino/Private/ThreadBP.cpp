// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4DuinoPCH.h"
#include "ThreadTest.h"
#include "ThreadBP.h"

CSerial *UThreadBP::SP = new CSerial();
UThreadBP *UThreadBP::BP = NewObject<UThreadBP>();
FString returnIt;
FString sendIt;
FString testStr;
bool serialShutdown;
bool threadShutdown;
bool threadIsOpen;
bool serialIsOpen;

UThreadBP::UThreadBP(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{ 

}

void UThreadBP::OpenSerial(int32 portNum, int32 baudRate)
{
	BP->StartThread(portNum, baudRate);
}

void UThreadBP::CloseSerial()
{
	ThreadTest::Shutdown();

	if (threadShutdown)
	{
		UE_LOG(LogTemp, Log, TEXT("THREAD CLOSED!!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ERROR: THREAD NOT CLOSED!!"));
	}

	if (serialShutdown)
	{
		UE_LOG(LogTemp, Log, TEXT("SERIAL PORT CLOSED!!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ERROR: SERIAL PORT NOT CLOSED!!"));
	}
}

FString UThreadBP::GetData(FString data)
{
	returnIt = data.TrimTrailing();
	return data;
}

FString UThreadBP::ReadSerial()
{
	return returnIt;
}

void UThreadBP::WriteSerial(FString writeData)
{
	sendIt = writeData;
}

FString UThreadBP::SendData()
{
	FString temp = sendIt;
	sendIt = "";
	return temp;
}

void UThreadBP::StartThread(int32 portNum, int32 baudRate)
{
	if (ThreadTest::ThreadLauncher(this, portNum, baudRate) != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("THREAD STARTED!!"));
		threadIsOpen = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ERROR: THREAD NOT STARTED!!"));
		threadIsOpen = false;
	}
	if (serialIsOpen)
	{
		UE_LOG(LogTemp, Log, TEXT("SERIAL PORT OPENED!!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ERROR: SERIAL PORT NOT OPENED!!"));
	}
}


bool UThreadBP::IsSerialPortOpen()
{
	if (serialIsOpen && threadIsOpen)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UThreadBP::IsThreadClosed(bool threadClosed)
{
	threadShutdown = threadClosed;
}

void UThreadBP::IsSerialClosed(bool serialClosed)
{
	serialShutdown = serialClosed;
}

void UThreadBP::SerialOpened(bool serialOpen)
{
	serialIsOpen = serialOpen;
}

//FString UThreadBP::TestWriteRuturn()
//{
//	return testStr;
//}
//
//void UThreadBP::GetWriteRuturn(FString strData)
//{
//	testStr = strData;
//}