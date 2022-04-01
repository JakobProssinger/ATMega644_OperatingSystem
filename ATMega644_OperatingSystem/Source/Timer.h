/*
 * Timer.c
 *
 * Created: 09/03/2022 14:28:48
 * Author : jakob
 */ 

#ifndef TIMER_H
#define TIMER_H
#include "HTLStddef.h"
#include "TimerInterface.h"

void 
TTimerSetInterface ( TTimerInterface aInterface);

TBool
TimerInit( TTimerInitParams aParams );
void
TimerGetInfo( char * aString);

void 
TimerStart ( void );

void
TimerStoop( void );
#endif