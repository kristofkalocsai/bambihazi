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
* clock frequency: external xtal, 8MHz
*
*
*
*
*
**************************************************************************************************************
*/


#include "mcu_avr_atmega128_api.h" 	// MCU API   
#include "dpy_trm_s01.h"			// DPY API
#include <stdlib.h>					// for rand()


#define GAME_NUM	50
#define SCALE		8

unsigned long			cycle_counter = 0;
unsigned int			led_counter=0;
volatile unsigned int	ms_counter = 0;
unsigned char			err;
unsigned char 			game = 1;
float					temp_sensor;
unsigned char			but1, but2, but3;
unsigned int			randnum;
unsigned int			randnum1000;
unsigned int			randnum1500;



/********  function prototypes  ***************************/
void Timer0_Init(void);
void Timer1_Init(void);
ISR(SIG_OVERFLOW0);
ISR(TIMER1_COMPA_vect);
int main(void);

/********  Timer0 overflow IT Service Routine  ***************************/
ISR(SIG_OVERFLOW0) // Timer0 overflow
{
   led_counter++; 
   if (led_counter<15) DPY_TRM_S01__LED_4_OFF();
   else if (led_counter<30) DPY_TRM_S01__LED_4_ON();
   else {
		err=dpy_trm_s01__Temp_ReadTEMP(&temp_sensor,TMP75_JUMPER_OFF,TMP75_JUMPER_OFF,TMP75_JUMPER_OFF);
							/* Reads the temperature sensor */
		randnum = rand()/100;
		dpy_trm_s01__7seq_write_number(temp_sensor,1);	/* Writes the temperature data to the    */
		dpy_trm_s01__7seq_write_number(ms_counter,0);
		ms_counter=0;
		led_counter=0;
		}
}

/********  Timer1 output compare IT SR ***********/
ISR(TIMER1_COMPA_vect)
{
	ms_counter++;
	TCNT1H = 0x00;		//set counter initial value
	TCNT1L = 0x00;		//
	 
}

/********  main program  ***************************/
int main (void)
{
   dpy_trm_s01__Init();		// Initialize the DPY dysplay card
   //Timer0_Init();			// Initialize timer0
   Timer1_Init();			// timer1 init
//   dpy_trm_s01__Temp_Init(TMP75_JUMPER_OFF,TMP75_JUMPER_OFF,TMP75_JUMPER_OFF);	
							/* Initialisation of temp. sensor */ 
 
   SYS_LED_DIR_OUTPUT();	// Set the pin driving the system led to output
   SYS_LED_ON();			// Switch on system led
   sei();					// enable interrupts
//   temp_sensor=24.3; 


////////////GAME///////////////
   while(game < GAME_NUM+1)
   {
		//game start
		ms_counter = 0;
		randnum = rand();
		randnum = randnum / SCALE; //rand() generates 0 to 32767 /SCALE ~4000
		randnum1000 = randnum + 1000;
		randnum1500 = randnum + 1500;
	
		while(ms_counter < 1000)
		{
			
		}
	
		while(ms_counter < 5000)
		{
		  /*DPY_TRM_S01__LED_4_OFF();
			led_counter++;
			dpy_trm_s01__7seq_write_number(led_counter,0);
			ms_counter = 0;
			cycle_counter = 0; */

			
			while(ms_counter <= randnum1500)
			{
				if(ms_counter == randnum1000)
				{
					DPY_TRM_S01__LED_4_ON();
				}
				if(ms_counter == randnum1500)
				{
					DPY_TRM_S01__LED_4_OFF();
				}
				else break;
			}
		}

		game++;

   }

}


/********  Timer0 initialisation  ***************************/
void Timer0_Init(void)
{
   TCCR0=0x07;			// Set TIMER0 prescaler to CLK/1024                 
   TCNT0=0;				// Set the counter initial value                    
   TIMSK=_BV(TOIE0);	// Enable TIMER0 overflow interrupt                 
}

/*******  Timer1 init ***************************************/
void Timer1_Init(void)
{
	TCCR1A = 0x00;		//default
	TCCR1B = 0x02;		//prescaler to /8 -> 1Mhz -> 1us
	TCCR1C = 0x00;		//no force output compare
	
	TCNT1H = 0x00;		//set counter initial value
	TCNT1L = 0x00;		//
	
	OCR1AH = 0x03;		//set output compare to 1000dec
	OCR1AL = 0xE8;		//1000us -> 1ms
	
	TIMSK=_BV(OCIE1A);	//enable TIMER1 output-compare interrupt
}

