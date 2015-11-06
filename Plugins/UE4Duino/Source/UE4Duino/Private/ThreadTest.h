#pragma once
#include "UE4DuinoPCH.h"
//#include "ThreadPlayerController.h"
#include "SerialClass.h"
#include "ThreadBP.h"

/**
*
*/
class ThreadTest : public FRunnable
{

	static ThreadTest* Runnable;

	static UThreadBP* BP;

	FString* theData;

	UThreadBP* TheBP;

	FRunnableThread* Thread;

	FThreadSafeCounter StopTaskCounter;

	static CSerial *SP;

public:

	ThreadTest(UThreadBP* IN_BP);
	virtual ~ThreadTest();

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	void EnsureCompletion();

	static ThreadTest* ThreadLauncher(UThreadBP* IN_BP, int32 portNum, int32 baudRate);

	static void Shutdown();

};
