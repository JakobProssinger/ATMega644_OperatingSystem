/*
 * OsBackgroundTask.c
 *
 * Created: 09/03/2022 14:28:48
 * Author : Jakob
 */ 
#include <string.h>
#include <stdlib.h>
#include "OsBackgroundTask.h"
#include "HTLStddef.h"


/******** Definition of global variables********/

static TBgtListHeader BgList;

void
OSBackgroundTaskInit( void )
{
	 memset( &BgList, 0, sizeof(BgList)); // allocate and set to 0
	
}

TBool
OSBackgroundTaskAddTask( TBgFunction	aFunction,
						 void *			aUserData)
{
	TBgElement bgElement;
	TBgElement nextElement;
	bgElement = calloc ( sizeof( * bgElement), 1);
	if ( ! bgElement )
		return EFALSE;
	
	if ( BgList.First )
	{
		nextElement = BgList.First;
		while (nextElement != NULL)
			nextElement = nextElement->Next;
			
		nextElement->Next = bgElement;
	} else {
		BgList.First = bgElement;
	}
	
	bgElement->Function = aFunction;
	bgElement->UserData = aUserData;
	
	return ETRUE;
}

TBool
OSBackgroundTaskRemove(  TBgFunction	aFunction,
						 void *			aUserData )
{
	if (BgList.First == NULL)
		return EFALSE;
		
	TBgElement nextElement = BgList.First;
	while (nextElement)
	{
		if(nextElement->Function == aFunction 
			&& nextElement->Function == aUserData)
		{
			
		}
		nextElement = nextElement->Next;
		if(nextElement->Next == NULL)
			return EFALSE;
		
	}
	
	
	return ETRUE;
}

void
OSBackgroundTaskExecute( void )
{
	if ( !BgList.ExecuteNext ) // End of loop
		BgList.ExecuteNext = BgList.First;
		
	if ( BgList.ExecuteNext )
	{
		BgList.ExecuteNext->Function(BgList.ExecuteNext->UserData);
		BgList.ExecuteNext = BgList.ExecuteNext->Next;
	}
		
}

