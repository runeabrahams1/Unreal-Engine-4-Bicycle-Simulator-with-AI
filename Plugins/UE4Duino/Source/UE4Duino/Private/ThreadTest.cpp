// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4DuinoPCH.h"
#include "ThreadTest.h"
#include "ThreadBP.h"

ThreadTest* ThreadTest::Runnable = NULL;

CSerial *ThreadTest::SP = new CSerial();

UThreadBP *ThreadTest::BP = NewObject<UThreadBP>();

char incomingData[1] = {};			// don't forget to pre-allocate memory
char outData[250] = {};
char writeBuffer[250] = {};
int len = 0;
int32 thePort;
int32 theBaud;

ThreadTest::ThreadTest(UThreadBP* IN_BP)
	: TheBP(IN_BP)
	, StopTaskCounter(0)

{
	Thread = FRunnableThread::Create(this, TEXT("ThreadTest"), 0, TPri_BelowNormal);
}

ThreadTest::~ThreadTest()
{
	delete Thread;
	Thread = NULL;
}

bool ThreadTest::Init()
{
	// TODO: Initialization

	return true;
}

uint32 ThreadTest::Run()
{
	FPlatformProcess::Sleep(0.03);
	while (StopTaskCounter.GetValue() == 0 && SP->IsOpened())
	{
		
		int nBytesRead = SP->ReadData(incomingData, 1);
		if (nBytesRead > 0)
		{
			if (strcmp(incomingData, "\n") == 0)
			{

				if (strlen(outData) > 0)
				{
					std::string test(outData);
					FString Fout(test.c_str());
					TheBP->GetData(Fout);
					len = 0;
					memset(outData, 0, sizeof outData);
				}
			}
			else
			{
				outData[len] = incomingData[0];
				len++;
			}
		}
		FString writeData = TheBP->SendData();
		strcpy(writeBuffer, TCHAR_TO_ANSI(*writeData));
		SP->SendData(writeBuffer, strlen(writeBuffer));
	}
	return 0;
}

void ThreadTest::Stop()
{
	StopTaskCounter.Increment();
}

ThreadTest* ThreadTest::ThreadLauncher(UThreadBP* IN_BP, int32 portNum, int32 baudRate)
{
	thePort = portNum;
	theBaud = baudRate;
	if (SP->Open(thePort, theBaud))
	{
		BP->SerialOpened(true);
	}
	else
	{
		BP->SerialOpened(false);
	}
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		
		Runnable = new ThreadTest(IN_BP);
	}
	
	return Runnable;
}

void ThreadTest::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void ThreadTest::Shutdown()
{

	if (SP->Close())
	{
		BP->IsSerialClosed(true);
	}
	else
	{
		BP->IsSerialClosed(false);
	}
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
	if (Runnable == NULL)
	{
		BP->IsThreadClosed(true);
	}
	else
	{
		BP->IsThreadClosed(false);
	}
	
}
