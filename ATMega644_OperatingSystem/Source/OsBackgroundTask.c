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
	bgElement = calloc ( sizeof( * bgElement), 1);
	if ( ! bgElement )
		return EFALSE;
		
	TBgElement ptr = BgList.First;
	while (ptr != NULL)
	{
		ptr = ptr->next;
	}
	ptr->next = bgElement;
	return ETRUE;
}

TBool
OSBackgroundTaskRemove(  TBgFunction	aFunction,
						 void *			aUserData )
{
	if (BgList.First == NULL)
		return EFALSE;
		
	TBgElement ptr = BgList.First;
	while (ptr != NULL)
	{
		ptr = ptr->next;
		if(ptr->next == NULL)
			return EFALSE;
	}
	
	
	
	return ETRUE;
}

TBool
OSBackgroundTaskExecute( void )
{
	if (BgList.First == NULL)
	{
		return EFALSE;
	}
	BgList.First->Function(BgList.First->UserData);
	
	return ETRUE;
}

