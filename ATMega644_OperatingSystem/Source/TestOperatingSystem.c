/*
 * TestOperatingSystem.c
 *
 * Created: 09/03/2022 14:28:48
 * Author : jakob
 */ 

#include <avr/io.h>
#include "Os.h"

void TestBgTask1(void);

void TestBgTask2(void);

void TestTask1(void * UserData);

int main(void)
{
    while (1) 
    {
		TestBgTask2();
    }
}

typedef struct 
{
	unsigned int Counter;
}TUserData1;  

void TestTask1(void * aUserData){
	TUserData1 * UserData = ( TUserData1 * ) aUserData;
	
	UserData->Counter++;
}

void TestBgTask2(void)
{
	OsInit();
	TUserData1 UserData1;
	UserData1.Counter = 0;
	TUserData1 UserData2;
	UserData2.Counter = 0;
	TUserData1 UserData3;
	UserData3.Counter = 0;
	
	unsigned int j = 0;
	
	while( j < 100 )
	{
		unsigned int i = 0;
		OSBackgroundTaskAddTask( TestTask1, &UserData1 );
		OSBackgroundTaskAddTask( TestTask1, &UserData2 );
		OSBackgroundTaskAddTask( TestTask1, &UserData3 );
	
		for (i = 0; i < 5; i++)
		OSBackgroundTaskExecute();
	
		OSBackgroundTaskRemove( TestTask1, &UserData2 );
	
		for (i = 0; i < 5; i++)
		OSBackgroundTaskExecute();
	
		OSBackgroundTaskRemove( TestTask1, &UserData1 );
	
		for (i = 0; i < 5; i++)
		OSBackgroundTaskExecute();
	
		OSBackgroundTaskRemove( TestTask1, &UserData3 );
		
		j++;
	}
	if(UserData1.Counter) // REMOVED BY COMPIPLER BECAUSE UserData1 is not used anymore
	{
		UserData1.Counter++;
	}
	
}

void TestBgTask1(void)
{
	OsInit();
	
	TUserData1 UserData1;
	UserData1.Counter = 0;
	TUserData1 UserData2;
	UserData2.Counter = 0;
	TUserData1 UserData3;
	UserData3.Counter = 0;
	
	OSBackgroundTaskAddTask( TestTask1, &UserData1 );
	OSBackgroundTaskAddTask( TestTask1, &UserData2 );
	OSBackgroundTaskAddTask( TestTask1, &UserData3 );

	
	while ( 1 )
	{
		OSBackgroundTaskExecute();
		
		if(UserData1.Counter == 1) // delete first
		{
			OSBackgroundTaskRemove(TestTask1, &UserData1);
			UserData1.Counter++;
		}
		
		if(UserData2.Counter == 2) //delete second
		{
			OSBackgroundTaskRemove(TestTask1, &UserData2);
			UserData2.Counter++;
		}
		
		if(UserData3.Counter == 3) // delete third
		{
			OSBackgroundTaskRemove(TestTask1, &UserData3);
			UserData3.Counter++;
		}
	}
}
