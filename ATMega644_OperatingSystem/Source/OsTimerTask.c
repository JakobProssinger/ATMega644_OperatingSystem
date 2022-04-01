  /*
   * OsTimerTask.c
   *
   * Created: 09.03.2022 14:28:56
   * Author : jakob
   */ 
  
#include <string.h>
#include <stdlib.h>
#include "HtlStddef.h"
#include "OsTimerTask.h"
  
TBool OsTimerTaskInit(	unsigned long aCpuClk,
						unsigned long aTimerInterval)
{
	return ETRUE;
}
    
TBool OsTimerTaskAddTask(
	TTimerTaskFunction* aFunction,
	void*				aUserdata)
{
	
	return ETRUE;
}

TBool OsTimerTaskRemoveTask(
	TTimerTaskFunction* aFunction,
	void*				aUserdata)
{

	return ETRUE;
}

void OsTimerTaskStart()
{
}

void OsTimerTaskStop()
{
}