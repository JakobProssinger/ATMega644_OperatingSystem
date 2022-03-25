/*
 * TimerInterface.h *
 * Created: 09/03/2022 14:28:48
 * Author : jakob
 */ 

#ifndef TIMERINTERFACE_H
#define  TIMERINTERFACE_H

#include <avr/io.h>
#include "HTLStddef.h"

typedef void (*TTimerFunction)(void* aUserData); //Function pointer

typedef struct 
{
	unsigned long CpuClock;
	unsigned long Resolution;
}TTimerInitParams;

typedef TBool (*TTimerInit)(TTimerInitParams); //Function pointer

typedef TBool (*TTimerSetInterruptFunction)(	TTimerFunction * aFunction,
												void * aUserData);	//Function pointer

typedef void (*TTimerStart)( void );	//Function pointer

typedef void (*TTimerStop)( void );	//Function pointer


typedef struct
{
	TTimerInit					init; //init function
	TTimerSetInterruptFunction	SetFunction;
	TTimerStart					StartFunction;
	TTimerStop					StopFunction;
}TTimerInterface;

#endif