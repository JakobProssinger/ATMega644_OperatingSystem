/*
 * OsBackgroundTask.h
 *
 * Created: 09/03/2022 14:28:48
 * Author : jakob
 */ 


#ifndef OSBACKGROUNDTAKS_H
#define OSBACKGROUNDTAKS_H

#include "HTLStddef.h"

typedef struct TBgElementStruct* TBgElement;

typedef void (*TBgFunction)(void* aUserData); // function pointer

struct TBgElementStruct
{
	TBgElement Next;
	TBgFunction Function;
	void * UserData;
};

typedef struct
{
	TBgElement First;
	TBgElement ExecuteNext;
	unsigned int Size;
}TBgtListHeader;


void
OSBackgroundTaskInit( void );

TBool
OSBackgroundTaskAddTask( TBgFunction	aFunction,
						  void *		aUserData);

TBool
OSBackgroundTaskRemove( TBgFunction	aFunction,
						void *		aUserData);

void
OSBackgroundTaskExecute( void );

#endif