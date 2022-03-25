  /*
   * OsTimerTask.c
   *
   * Created: 09.03.2022 14:28:
   * Author : jakob
   */ 
  
#include <string.h>
#include <stdlib.h>
#include "HtlStddef.h"
#include "OsTimerTask.h"
  
OsTimerTaskInit(
unsigned long aCpuClk,
unsigned long aTimerInterval
);
    
OsTimerTaskAddTask();
OsTimerTaskRemoveTask();
  
OsTimerTaskStart();
OsTimerTaskStop();