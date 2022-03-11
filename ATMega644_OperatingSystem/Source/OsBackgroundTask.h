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
	TBgElement next;
	TBgFunction Function;
	void * UserData;
};

typedef struct
{
	TBgElement First;

}TBgtListHeader;


void
OSBackgroundTaskInit( void );

TBool
OSBackgroundTaskAddTask( TBgFunction	aFunction,
						  void *		aUserData);

TBool
OSBackgroundTaskRemove( TBgFunction	aFunction,
						void *		aUserData);

TBool
OSBackgroundTaskExecute( void );

#endif