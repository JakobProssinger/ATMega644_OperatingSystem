  /*
   * OsTimerTask.h
   *
   * Created: 09.03.2022 14:28:57
   * Author : jakob
   */ 
 
#ifndef OSTIMERTASK_H
#define OSTIMERTASK_H

void OsTimerTaskInit(
unsigned long aCpuClk,
unsigned long aTimerInterval
);

void OsTimerTaskAddTask();
void OsTimerTaskRemoveTask();

void OsTimerTaskStart();
void OsTimerTaskStop();

#endif