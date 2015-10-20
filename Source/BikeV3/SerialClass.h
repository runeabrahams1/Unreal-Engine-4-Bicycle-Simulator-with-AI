// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define ARDUINO_WAIT_TIME 2000

/**
 * 
 */
class BIKEV3_API SerialClass
{
private:
	//Serial comm handler
	HANDLE hSerial;
	//Connection status
	bool connected;

	LPDWORD errors;
	//Get various information about the connection
	COMSTAT status;
public:
	//Initialize Serial communication with the given COM port
	SerialClass(char *portName);
	//Close the connection
	~SerialClass();
	//Read data in a buffer, if nbChar is greater than the
	//maximum number of bytes available, it will return only the
	//bytes available. The function return -1 when nothing could
	//be read, the number of bytes actually read.
	int ReadData(char *buffer, unsigned int nbChar);
	//Writes data from a buffer through the Serial connection
	//return true on success.
	bool WriteData(char *buffer, unsigned int nbChar);
	//Check if we are actually connected
	bool IsConnected();


};
