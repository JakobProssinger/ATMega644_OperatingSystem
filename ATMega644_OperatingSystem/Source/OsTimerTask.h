  /*
   * OsTimerTask.h
   *
   * Created: 09.03.2022 14:28:57
   * Author : jakob
   */ 
 
#ifndef OSTIMERTASK_H
#define OSTIMERTASK_H

TBool OsTimerTaskInit(
unsigned long aCpuClk,
unsigned long aTimerInterval
);

TBool OsTimerTaskAddTask();
TBool OsTimerTaskRemoveTask();

TBool OsTimerTaskStart();
TBool OsTimerTaskStop();

#endif