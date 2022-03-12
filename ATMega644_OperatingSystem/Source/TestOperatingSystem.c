/*
 * TestOperatingSystem.c
 *
 * Created: 09/03/2022 14:28:48
 * Author : jakob
 */ 

#include <avr/io.h>
#include "Os.h"

void TestBgTask1(void);

void TestTask1(void * UserData);

int main(void)
{
    while (1) 
    {
		TestBgTask1();
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

void TestBgTask1(void)
{
	OsInit();
	
	TUserData1 UserData1;
	UserData1.Counter = 0;
	TUserData1 UserData2;
	UserData2.Counter = 5;
	OSBackgroundTaskAddTask( TestTask1, &UserData1 );
	OSBackgroundTaskAddTask( TestTask1, &UserData2 );

	
	while ( 1 )
	{
		OSBackgroundTaskExecute();
		
		if(UserData1.Counter == 5)
		{
			OSBackgroundTaskRemove(TestTask1, &UserData1);
			UserData1.Counter++;
		}
		
		if(UserData2.Counter == 7)
		{
			OSBackgroundTaskRemove(TestTask1, &UserData2);
			UserData2.Counter++;
		}
	}
}
