/****************************************************
  File: Timer644.c
  Description:
    Timer interface for the AtMega644PA microcontroller.
  
  Restrictions:
*****************************************************/
#include "Timer644.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "TimerInterface.h"

#define TIMER_MAX_PRESCALER_INDEX 6
unsigned int Timer644PrescalerValues[TIMER_MAX_PRESCALER_INDEX] =
{ 1, 1, 8, 64, 256, 1024 };
  
unsigned int Timer644OverflowCounter = 0;
unsigned int Timer644CompareMatchCounter = 0; 

/* Interrupt function pointer for the timer */
TTimerFunction Timer644FunctionTimer0Ctc;
void *         Timer644FunctionTimer0CtcUserData;

TTimerFunction Timer644FunctionTimer1Ctc;
void *         Timer644FunctionTimer1CtcUserData;
    
TTimerFunction Timer644FunctionTimer1Capture;
void *         Timer644FunctionTimer1CaptureUserData;


/**** Declaration of private functions */
static TBool
Timer644Init_(
TTimerInitParams aParams);

static TBool
Timer644Init0(
unsigned long aCpuClk,
unsigned long aTimerIntervall,
TTimer644Mode    aMode );

static TBool
Timer644Init1(
unsigned long aCpuClk,
unsigned long aTimerIntervall,
TTimer644Mode    aMode );

static void
Timer644CalculatePrescalerNormalMode(
  unsigned long   aCpuClk,
  unsigned long   aTimerIntervall,
  unsigned char * aPrescaler );

static void
Timer644CalculatePrescalerTimer1NormalMode(
unsigned long   aCpuClk,
unsigned long   aTimerIntervall,
unsigned char * aPrescaler );


static void
Timer644CalculateCompareRegister(
unsigned long   aCpuClk,
unsigned long   aTimerIntervall,
unsigned char * aCompareRegister,
unsigned char * aPrescaler );

static void
Timer644CalculateCompareRegisterTimer1(
unsigned long   aCpuClk,
unsigned long   aTimerIntervall,
unsigned int  * aCompareRegister,
unsigned char * aPrescaler );

static TBool
  Timer644SetInterruptFunction_(
  TTimerFunction    aFunction,
  void *            aUserData );

void
Timer644GetInterface(
  TTimerInterface * aInterface )
{
  aInterface->init					= Timer644Init_;
  aInterface->SetFunction			= Timer644SetInterruptFunction_;
  aInterface->StartFunction			= 0;
  aInterface->StopFunction			= 0;
}

static TBool
Timer644Init_( 
 TTimerInitParams aParams)
 {
   return Timer644Init( aParams.CpuClock, aParams.Resolution, TIMER_MODE_CTC, TIMER_NO_0 );
 }


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
  TTimer644Number  aTimerNo )
{
  TBool success;
	switch( aTimerNo )
  {
  case TIMER_NO_0:
    success = Timer644Init0( aCpuClk, aTimerIntervall, aMode );  
    break;
      
  case TIMER_NO_1:
    success = Timer644Init1( aCpuClk, aTimerIntervall, aMode );
    break;
  
  default:
    return EFALSE;
    break;     
  }
  
  return success;
}

static TBool
Timer644SetInterruptFunction_(
  TTimerFunction    aFunction,
  void *            aUserData )
{
  return Timer644SetInterruptFunction(
    aFunction,
    aUserData,
    TIMER_MODE_CTC,
    TIMER_NO_0 );
}

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
  TTimer644Number   aTimerNo )
{
	switch( aTimerNo )
  {
  case TIMER_NO_0:
    switch( aMode )
    {
    case TIMER_MODE_CTC:
      Timer644FunctionTimer0Ctc         = aFunction;
      Timer644FunctionTimer0CtcUserData = aUserData;
      break;      
    default:
      return EFALSE;
      break;
    }
    break;
      
  case TIMER_NO_1:
    switch( aMode )
    {
    case TIMER_MODE_CTC:
      Timer644FunctionTimer1Ctc         = aFunction;
      Timer644FunctionTimer1CtcUserData = aUserData;      
      break;
      
    case TIMER_MODE_INPUT_CAPTURE_ICP1:
      Timer644FunctionTimer1Capture         = aFunction;
      Timer644FunctionTimer1CaptureUserData = aUserData;
      break;  
    default:
      return EFALSE;
      break;
    }
    break;
  
  default:
    return EFALSE;
    break;     
  }
  
  return ETRUE;
}



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
  TCompareOutput      aCompareOutput,
  TTimer644OutputMode aOutputMode,
  float               aPwmLevel )
{
  TBool success = ETRUE;
  unsigned char comp;
  
	switch( aTimerNo )
  {
  case TIMER_NO_0:
    comp = ( unsigned char )( 255 * aPwmLevel / 100 );
    switch( aCompareOutput )
    {
    case COMP_A:
      TCCR0A &= ~( 3 << COM0A0 );
      OCR0A = comp;
      switch( aOutputMode )
      {
      case TIMER_OUTPUT_NORMAL:
        TCCR0A |= ( 2 << COM0A0 );
        TCCR0B &= ~( 1 << WGM02 );
        break;
      case TIMER_OUTPUT_INVERTED:
        TCCR0A |= ( 3 << COM0A0 );
        TCCR0B &= ~( 1 << WGM02 );
        break;
      case TIMER_OUTPUT_TOGGLE:
        TCCR0A |= ( 1 << COM0A0 );
        TCCR0B |= ( 1 << WGM02 );
        break;
      }
      TIMSK0 |=  ( 1 << OCIE0A );        // Compare Match interrupt, only for testing
      DDRB   |=  ( 1 << DDB3 );
      break;  
    case COMP_B:
      OCR0B = comp;
      TCCR0A &= ~( 3 << COM0B0 );
      switch( aOutputMode )
      {
      case TIMER_OUTPUT_NORMAL:
        TCCR0A |= ( 2 << COM0B0 );
        break;
      case TIMER_OUTPUT_INVERTED:
        TCCR0A |= ( 3 << COM0B0 );
        break;
      case TIMER_OUTPUT_TOGGLE:
        success = EFALSE;
        break;
      }
      TIMSK0 |=  ( 1 << OCIE0B );        // Compare Match interrupt, only for testing
      DDRB   |=  ( 1 << DDB4 );
      break;
    }
    
    break;
      
  default:
    return EFALSE;
    break;     
  }
  
  return success;
}



/***********************************************************************
  Function: Timer644Init0
  Description:
    Initialize the timer TIMER_NO_0 of the AtMega644 controller.
  Parameters:
    aCpuClk         - The clock frequency of the controller
    aTimerIntervall - The interrupt interval of the timer in us.
	  aMode           - The mode of the timer
************************************************************************/
static TBool
Timer644Init0(
  unsigned long aCpuClk,
  unsigned long aTimerIntervall,
  TTimer644Mode    aMode )
{
  unsigned char compareRegister;
  unsigned char prescaler;
 
  TCCR0A = 0;
  TCCR0B = 0;  
  TIMSK0 = 0;      /* Timer interrupt mask register */
  TIFR0  = 0x07;   /* Clear all interrupt flags */
  Timer644FunctionTimer0Ctc = 0;
  
  switch( aMode)
  {
  case TIMER_MODE_NORMAL:
    Timer644CalculatePrescalerNormalMode( aCpuClk, aTimerIntervall, &prescaler );
    TCCR0B |=  ( prescaler << CS00 );
    TIMSK0 |=  ( 1 << TOIE0 );
    break;

  case TIMER_MODE_CTC:
    Timer644CalculateCompareRegister( aCpuClk, aTimerIntervall,
      &compareRegister, &prescaler );
    OCR0A = compareRegister;
    TCCR0B |= ( prescaler << CS00 );
    TCCR0A |= ( 1 << WGM01 );          // Timer mode CTC
    TIMSK0 |=  ( 1 << OCIE0A );        // Compare Match interrupt
    break;
    
  case TIMER_MODE_FAST_PWM:
    Timer644CalculatePrescalerNormalMode( aCpuClk, aTimerIntervall, &prescaler );
    TCCR0B |=  ( prescaler << CS00 );
    TCCR0A |= ( 3 << WGM00 );          // Timer mode Fast PWM
    TIMSK0 |=  ( 1 << TOIE0 );         // Overflow interrupt for testing
    
    break;
  case TIMER_MODE_PHASE_CORRECT_PWM:
    Timer644CalculatePrescalerNormalMode( aCpuClk, aTimerIntervall, &prescaler );
    TCCR0B |= ( prescaler << CS00 );
    //TCCR0A |= ( 1 << COM0A1 ) | ( 1 << COM0A0 );
    TCCR0A |=  ( 1 << WGM00 );          // Phase correct PWM
    TIMSK0 |=  ( 1 << TOIE0 );         // Overflow interrupt for testing
    TIMSK0 |=  ( 1 << OCIE0A );        // Compare match A interrupt for testing
    TIMSK0 |=  ( 1 << OCIE0B );        // Compare match A interrupt for testing
    break;  
  default:
    return EFALSE;
    break;
  }
  sei();
  return ETRUE;
}

/***********************************************************************
  Function: Timer644Init1
  Description:
    Initialize the 16-Bit timer TIMER_NO_1 of the AtMega644 controller.
  Parameters:
    aCpuClk         - The clock frequency of the controller
    aTimerIntervall - The interrupt interval of the timer in us.
	  aMode           - The mode of the timer
************************************************************************/
static TBool
Timer644Init1(
  unsigned long aCpuClk,
  unsigned long aTimerIntervall,
  TTimer644Mode    aMode )
{
  unsigned int compareRegister;
  unsigned char prescaler;
 
  TCCR1A = 0;
  TCCR1B = 0;  
  TIMSK1 = 0;      /* Timer interrupt mask register */
  TIFR1  = 0x07;   /* Clear all interrupt flags */
  Timer644FunctionTimer1Ctc = 0;
  
  switch( aMode)
  {
 #if 1
  case TIMER_MODE_NORMAL:
    Timer644CalculatePrescalerTimer1NormalMode( aCpuClk, aTimerIntervall, &prescaler );
    TCCR1B |=  ( prescaler << CS10 );
    TIMSK1 |=  ( 1 << TOIE1 );
    break;

  case TIMER_MODE_CTC:
    Timer644CalculateCompareRegisterTimer1( aCpuClk, aTimerIntervall,
      &compareRegister, &prescaler );
    OCR1A  = compareRegister;
    TCCR1B |= ( prescaler << CS00 );
    TCCR1B |= ( 1 << WGM12 );          // Timer mode CTC
    TIMSK1 |=  ( 1 << OCIE1A );        // Compare Match interrupt
    break;
  

#endif
    
   case TIMER_MODE_INPUT_CAPTURE_ICP1:
     DDRD &= ~( 1 << DDD6 );
     Timer644CalculatePrescalerTimer1NormalMode( aCpuClk, aTimerIntervall, &prescaler );
     TCCR1B |=  ( prescaler << CS10 );
     TCCR1B |=  ( 1 << ICNC1 );  /* Noise canceler activated */
     TCCR1B |=  ( 1 << ICES1 );  /* rising edge */
     TIMSK1 |=  ( 1 << ICIE1 ); 
   
     break;
#if 0
  case TIMER_MODE_FAST_PWM:
    break;
  case TIMER_MODE_PHASE_CORRECT_PWM:
    break;
#endif
  
  default:
    return EFALSE;
    break;
  }
  sei();
  return ETRUE;
}

/***********************************************************************
  Function: Timer644CalculatePrescalerNormalMode
  Description:
    Calculate the prescaler value for the normal mode.
  Parameters:
    aCpuClk          - The clock frequency of the controller
    aTimerIntervall  - The interrupt interval of the timer in us.
    aPrescaler       - The calculated prescaler value
************************************************************************/
static void
Timer644CalculatePrescalerNormalMode(
  unsigned long   aCpuClk,
  unsigned long   aTimerIntervall,
  unsigned char * aPrescaler )
{
  float timerIntervall;
  unsigned char prescalerIndex;
  
  for ( prescalerIndex = TIMER_MAX_PRESCALER_INDEX - 1; 
    prescalerIndex > 1; prescalerIndex-- )
  {
    timerIntervall = ( 256.0 * 1000000 ) / 
      ( double )aCpuClk * Timer644PrescalerValues[prescalerIndex];
    
    if ( timerIntervall < aTimerIntervall )
      break;
  }
  
  *aPrescaler = prescalerIndex;
}

/***********************************************************************
  Function: Timer644CalculatePrescalerTimer1NormalMode
  Description:
    Calculate the prescaler value for the normal mode.
  Parameters:
    aCpuClk          - The clock frequency of the controller
    aTimerIntervall  - The interrupt interval of the timer in us.
    aPrescaler       - The calculated prescaler value
************************************************************************/
static void
Timer644CalculatePrescalerTimer1NormalMode(
  unsigned long   aCpuClk,
  unsigned long   aTimerIntervall,
  unsigned char * aPrescaler )
{
  float timerIntervall;
  unsigned char prescalerIndex;
  
  for ( prescalerIndex = TIMER_MAX_PRESCALER_INDEX - 1; 
    prescalerIndex > 1; prescalerIndex-- )
  {
    timerIntervall = ( 256.0 * 256.0 * 1000000 ) / 
      ( double )aCpuClk * Timer644PrescalerValues[prescalerIndex];
    
    if ( timerIntervall < aTimerIntervall )
      break;
  }
  
  *aPrescaler = prescalerIndex;
}

/***********************************************************************
  Function: Timer644CalculateCompareRegister
  Description:
    Calculate the prescaler value and the compare register value.
  Parameters:
    aCpuClk          - The clock frequency of the controller
    aTimerIntervall  - The interrupt interval of the timer in us.
	  aCompareRegister - The calculated compare register value for the timer
    aPrescaler       - The calculated prescaler value
************************************************************************/
static void
Timer644CalculateCompareRegister(
  unsigned long   aCpuClk,
  unsigned long   aTimerIntervall,
  unsigned char * aCompareRegister,
  unsigned char * aPrescaler )
{
  unsigned long ocra;
  unsigned char prescalerIndex;
  
  for ( prescalerIndex = 1; 
    prescalerIndex < TIMER_MAX_PRESCALER_INDEX;
    prescalerIndex++ )
  {
    ocra = ( aTimerIntervall * 
     ( ( double )aCpuClk / Timer644PrescalerValues[prescalerIndex] / 1000000.0 ))-1;
    if ( ocra <256)
      break;
  }
  
  *aCompareRegister = ocra;
  *aPrescaler       = prescalerIndex;  
}

/***********************************************************************
  Function: TimerCalculateCompareRegisterTimer6441
  Description:
    Calculate the prescaler value and the compare register value for Timer 1.
  Parameters:
    aCpuClk          - The clock frequency of the controller
    aTimerIntervall  - The interrupt interval of the timer in us.
	  aCompareRegister - The calculated compare register value for the timer
    aPrescaler       - The calculated prescaler value
************************************************************************/
static void
Timer644CalculateCompareRegisterTimer1(
  unsigned long   aCpuClk,
  unsigned long   aTimerIntervall,
  unsigned int  * aCompareRegister,
  unsigned char * aPrescaler )
{
  unsigned long ocra;
  unsigned char prescalerIndex;
  
  for ( prescalerIndex = 1; 
    prescalerIndex < TIMER_MAX_PRESCALER_INDEX;
    prescalerIndex++ )
  {
    ocra = ( aTimerIntervall * 
     ( ( double )aCpuClk / Timer644PrescalerValues[prescalerIndex] / 1000000.0 ))-1;
    if ( ocra < 65536 )
      break;
  }
  
  *aCompareRegister = ocra;
  *aPrescaler       = prescalerIndex;  
}

ISR( TIMER0_OVF_vect )
{
  Timer644OverflowCounter++;
}

ISR( TIMER0_COMPA_vect )
{
  Timer644CompareMatchCounter++;
  
  if ( Timer644FunctionTimer0Ctc != 0 )
    Timer644FunctionTimer0Ctc( Timer644FunctionTimer0CtcUserData );  
}

ISR( TIMER0_COMPB_vect )
{
  Timer644CompareMatchCounter++;
}

ISR( TIMER1_OVF_vect )
{
  Timer644OverflowCounter++;
}

ISR( TIMER1_COMPA_vect )
{
  Timer644CompareMatchCounter++;
  if ( Timer644FunctionTimer1Ctc )
    Timer644FunctionTimer1Ctc( Timer644FunctionTimer1CtcUserData );
}

