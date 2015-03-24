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


#define GAME_NUM		50
#define SCALE			8
#define WINDOW_START	1000
#define WINDOW_WIDTH	500

volatile unsigned int	ms_counter = 0;
unsigned char 			game = 1;
unsigned char			pressed;
unsigned int			randnum;
unsigned int			window_start;
unsigned int			window_end;
unsigned int 			t_delay;
signed   int			score = 0;



/********  function prototypes  ***************************/

void Timer1_Init(void);
ISR(SIG_OVERFLOW0);
ISR(TIMER1_COMPA_vect);
int main(void);



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

///////////GETTING READY////////////////

   dpy_trm_s01__Init();		// Initialize the DPY display card
   Timer1_Init();			// timer1 init
   SYS_LED_DIR_OUTPUT();	// Set the pin driving the system led to output
   SYS_LED_ON();			// Switch on system led
   sei();					// enable interrupts



////////////GAME///////////////

   while(game < GAME_NUM+1)
   {
		// game start
		// 1st second, generate rand, calculate hit window,
		// wait, turn off led1 if a previous game left it on

		ms_counter = 0;
		pressed = 0;					
		
		randnum = rand() / SCALE; 				//rand() generates 0 to 32767 /SCALE ~4000
		
		window_start = randnum + WINDOW_START;		//hit window start
		window_end = window_start + WINDOW_WIDTH;	// and end
		
		while(ms_counter < 1000)
		{

			if( !DPY_TRM_S01__BUTTON_2_GET_STATE() )
			{
				DPY_TRM_S01__LED_1_OFF();
				if( !pressed )
				{
					score = score - 20;				//pre-hit: -20p
					pressed = 1;
				}
			}
		}
		

		//seconds 1-5:

		while(ms_counter < 5000)
		{
			
			if( !DPY_TRM_S01__BUTTON_2_GET_STATE() )
			{
				if(ms_counter - t_delay > window_start + 200)//debouncing
				{
					DPY_TRM_S01__LED_1_OFF();		//switch off hit-led
				}
				if( !pressed )
				{
					score = score - 20;				//pre-post: -20p
					pressed = 1;
				}
			}
			

			//hit window
			while(ms_counter >= window_start && ms_counter <= window_end)
			{
				if(ms_counter == window_start)
				{
					dpy_trm_s01__7seq_write_3digit(0xFF,0xDF,0xFF);
				}
				if(ms_counter == window_end)
				{
					dpy_trm_s01__7seq_clear_dpy();
				}
				if( !DPY_TRM_S01__BUTTON_2_GET_STATE() )
				{	
					
					if(ms_counter - t_delay > window_start + 200)//debouncing
					{
						DPY_TRM_S01__LED_1_OFF();		//switch off hit-led
					}
					
					if( !pressed )
					{
						t_delay = ms_counter - window_start;	//hit!
						score = score + (500 - t_delay)/10;
						pressed = 1;
						DPY_TRM_S01__LED_1_ON();				//hit-led on
					}

				}
				else break;
			}
		}
		
		if(!pressed) score = score - 40;			// -40p for not trying at all
		
		game++;										 

   }
   dpy_trm_s01__7seq_write_number(score,0);


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

