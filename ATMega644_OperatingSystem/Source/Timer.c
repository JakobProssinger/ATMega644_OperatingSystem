/*
 * Timer.c
 *
 * Created: 09/03/2022 14:28:48
 * Author : jakob
 */ 

#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include "HTLStddef.h"
#include "TimerInterface.h"

typedef struct
{
	TTimerInterface	Interface;
	TTimerInitParams Params;

}TTimer;

static TTimer Timer;

void 
TTimerSetInterface ( TTimerInterface aInterface)
{
	memset(& Timer, 0, sizeof(TTimer)); // delete and init new

	Timer.Interface = aInterface;	 
}

TBool
TimerInit( TTimerInitParams aParams )
{
	if ( !Timer.Interface.init )
		return EFALSE;
		
	Timer.Params = aParams;
	return Timer.Interface.init(aParams);
}

void
TimerGetInfo( char * aString) // logging print
{
	sprintf(aString, "CLock: %lu, Interval: %lu",
	Timer.Params.CpuClock, Timer.Params.Resolution);
}

void 
TimerStart ( void )
{
	if( Timer.Interface.StartFunction)
		Timer.Interface.StartFunction();
}

void
TimerStoop( void )
{
	if ( Timer.Interface.StopFunction)
		Timer.Interface.StopFunction();

}