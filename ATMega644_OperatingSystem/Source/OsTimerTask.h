  /*
   * OsTimerTask.h
   *
   * Created: 09.03.2022 14:28:57
   * Author : jakob
   */ 
 
#ifndef OSTIMERTASK_H
#define OSTIMERTASK_H

typedef void (*TTimerTaskFunction)(
	void *			aUserData,
	unsigned long	aTimerTicks);

TBool OsTimerTaskInit(
	unsigned long aCpuClk,
	unsigned long aTimerInterval);

TBool OsTimerTaskAddTask( 
	TTimerTaskFunction* aFunction,
	void*				aUserdata);
	
TBool OsTimerTaskRemoveTask(
	TTimerTaskFunction* aFunction,
	void*				aUserdata);

void OsTimerTaskStart();

void OsTimerTaskStop();

#endif