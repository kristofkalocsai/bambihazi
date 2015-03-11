/*
**************************************************************************************************************
*                 Test program for the mitmót
*
* filename:	HF1.c
* created:	 
*
* prerequisites:
*			- WinAVR 20071221 is installed in the root of drive C (C:\WinAVR-20071221)
*
* function:	-reflextest
*
*
*
**************************************************************************************************************
*/


#include "mcu_avr_atmega128_api.h" 	// MCU API   
#include "dpy_trm_s01.h"			// DPY API
#include <stdlib.h>

//redefine rand() range for convenience
#undef RAND_MAX
#define RAND_MAX 999

unsigned char	led_counter=0;
unsigned char	err;
float			temp_sensor;
unsigned char	but1, but2, but3;
int			randnum;

/********  function prototypes  ***************************/
void Timer0_Init(void);
ISR(SIG_OVERFLOW0);
int main(void);

/********  Timer0 overflow IT Service Routine  ***************************/
ISR(SIG_OVERFLOW0) // Timer0 overflow
{
   led_counter++; 
   if (led_counter<15) DPY_TRM_S01__LED_4_OFF();
   else if (led_counter<30) DPY_TRM_S01__LED_4_ON();
   else {
		//err=dpy_trm_s01__Temp_ReadTEMP(&temp_sensor,TMP75_JUMPER_OFF,TMP75_JUMPER_OFF,TMP75_JUMPER_OFF);
							/* Reads the temperature sensor */
		randnum = rand();
		//dpy_trm_s01__7seq_write_number(temp_sensor,1);	/* Writes the temperature data to the    */
		dpy_trm_s01__7seq_write_number(randnum,0);
		led_counter=0;
		}
}

/********  main program  ***************************/
int main (void)
{
   dpy_trm_s01__Init();		// Initialize the DPY dysplay card
   Timer0_Init();			// Initialize timer0
   dpy_trm_s01__Temp_Init(TMP75_JUMPER_OFF,TMP75_JUMPER_OFF,TMP75_JUMPER_OFF);	
							/* Initialisation of temp. sensor */ 
   SYS_LED_DIR_OUTPUT();	// Set the pin driving the system led to output
   SYS_LED_ON();			// Switch on system led
   sei();					// enable interrupts
   temp_sensor=24.3; 
   while(1)
   {
//	_delay_ms(100);
	but1=DPY_TRM_S01__BUTTON_1_GET_STATE();
	but2=DPY_TRM_S01__BUTTON_2_GET_STATE();
	but3=DPY_TRM_S01__BUTTON_3_GET_STATE();
	if (but1) DPY_TRM_S01__LED_1_ON(); else DPY_TRM_S01__LED_1_OFF();
	if (but2) DPY_TRM_S01__LED_2_ON(); else DPY_TRM_S01__LED_2_OFF();
	if (but3) DPY_TRM_S01__LED_3_ON(); else DPY_TRM_S01__LED_3_OFF();
   }

}


/********  Timer0 initialisation  ***************************/
void Timer0_Init(void)
{
   TCCR0=0x07;			// Set TIMER0 prescaler to CLK/1024                 
   TCNT0=0;				// Set the counter initial value                    
   TIMSK=_BV(TOIE0);	// Enable TIMER0 overflow interrupt                 
}

