/****************************************************
  File: Timer644.h
  Description:
    Timer interface for the AtMega644PA microcontroller.
  
  Restrictions:
    - Only Normal and CTC mode supported
	  - Only Timer 0 supported
*****************************************************/
#ifndef TIMER_H
#define TIMER_H

#include "TimerInterface.h"

typedef enum
{
  TIMER_MODE_NORMAL,
  TIMER_MODE_CTC,
  TIMER_MODE_FAST_PWM,
  TIMER_MODE_PHASE_CORRECT_PWM,
  TIMER_MODE_INPUT_CAPTURE_ICP1  /* Inpute compare in: PD6 */  
} TTimer644Mode;

typedef enum
{
  TIMER_NO_0,
  TIMER_NO_1,
  TIMER_NO_2
} TTimer644Number;

typedef enum
{
  TIMER_OUTPUT_NORMAL,
  TIMER_OUTPUT_INVERTED,
  TIMER_OUTPUT_TOGGLE  
} TTimer644OutputMode;

typedef enum
{
  COMP_A,
  COMP_B
} TCompareOutput;

//      Rück-        Name des Typs     Übergabeparameter
//      gabewert
  
void
Timer644GetInterface(
  TTimerInterface * aInterface );  

/*****************************************************
  Function: Timer644Init
  Description:
    Initialize the timer of the AtMega644 controller.
  Parameters:
    aCpuClk         - The clock frequency of the controller
    aTimerIntervall - The interrupt interval of the timer
	  aMode           - The mode of the timer
	  aTimerNo        - The timer number of the controller
******************************************************/
TBool
Timer644Init( 
  unsigned long aCpuClk,
  unsigned long aTimerIntervall,
  TTimer644Mode    aMode,
  TTimer644Number  aTimerNo );
  
  
 
/*****************************************************
  Function: Timer644SetInterruptFunction
  Description:
    
  Parameters:
    aFunction - This function is called by the interrupt
    aUserData - This parameter is transfered to the function
	  aMode     - The mode of the timer
	  aTimerNo  - The timer number of the controller
    
  Return:
    ETRUE if the interrupt is supported for calling
    user functions otherwise EFALSE.
******************************************************/
TBool
Timer644SetInterruptFunction( 
  TTimerFunction    aFunction,
  void *            aUserData,
  TTimer644Mode     aMode,
  TTimer644Number   aTimerNo );


/*******************************************************************
  Function: Timer644SetPwmLevel
  Description:
  Parameters:
	  aTimerNo         - The timer number of the controller
    aCompareRegister - The compare output A or B
    aOutputMode      - Selects between normal, inverted and toggling
    aPwmLevel        - The PWM value in percent from 0 - 100                   
*******************************************************************/
TBool
Timer644SetPwmLevel( 
  TTimer644Number     aTimerNo,
  TCompareOutput   aCompareOutput,
  TTimer644OutputMode aOutputMode,
  float            aPwmLevel );        
  
#endif