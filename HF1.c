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
#include "eeprom_fun.h"				// EEPROM functions
#include <stdlib.h>					// for rand()


#define ROUND_NUM		50
#define RAND_MAX		3500
#define WINDOW_START	1000
#define WINDOW_WIDTH	500
#define ADDR_GAMENUM	0x0006
#define ADDR_SCORE_MIN	0x0007		//2 bytes for big scores
#define ADDR_SCORE_MAX  0x0009		//2 bytes
#define ADDR_SCORE_AVG	0x000B		//2 bytes

volatile unsigned int	ms_counter = 0;
unsigned char 			round = 1;
unsigned char			game;
unsigned char			pressed;
unsigned int			randnum;
unsigned int			window_start;
unsigned int			window_end;
unsigned int 			t_delay;
signed   int			score = 0;
signed   int			scoreMin = 0;
signed   int			scoreMax = 0;
signed   int			scoreAvg = 0;



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

	while(1)
	{

    	sei();			// enable interrupts

////////////GAME///////////////
   
	    while(round < ROUND_NUM+1)
	    {
			// game start
			// 1st second, generate rand, calculate hit window,
			// wait, turn off led1 if a previous game left it on
		
			ms_counter = 0;
			pressed = 0;					
		

			//seeding randnum with temp sensor data
			//randnum = srand(dpy_trm_s01__Temp_ReadTEMP(&temp_sensor,TMP75_JUMPER_OFF,TMP75_JUMPER_OFF,TMP75_JUMPER_OFF)) % RAND_MAX;
			randnum = rand() % RAND_MAX; 				//rand() generates 0 to ..., RAND_MAX
			//needs further random factor for true random number

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
			//game mechanics

			while(ms_counter < 5000)
			{
			
				if( !DPY_TRM_S01__BUTTON_2_GET_STATE() )	//btn push
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
					if(ms_counter == window_start)	//segment on
					{
						dpy_trm_s01__7seq_write_3digit(0xFF,0xDF,0xFF);
					}
					if(ms_counter == window_end)	//segment off
					{
						dpy_trm_s01__7seq_clear_dpy();
					}
					if( !DPY_TRM_S01__BUTTON_2_GET_STATE() ) //btn push
					{	
					
						if(ms_counter - t_delay > window_start + 200)//debouncing
						{
							DPY_TRM_S01__LED_1_OFF();		//switch off hit-led
						}
					
						if( !pressed )				//first press
						{
							t_delay = ms_counter - window_start;	//hit!
							score = score + ((500 - t_delay)/10);
							pressed = 1;
							DPY_TRM_S01__LED_1_ON();				//hit-led on
						}

					}
					else break;
				}
			}
		
			if(!pressed) score = score - 40;			// -40p for not trying at all
		
			round++;										 

	   }
	///////////////////GAME END/////////////////////////


	//needs interrupts disable somewhere here
		cli();


	///////////////////SCOREBOARD///////////////////////


		dpy_trm_s01__7seq_write_number(score,0);

		//game = EEPROM_read( @gamenum ) + 1;
		game = EEPROM_read( ADDR_GAMENUM ) + 1;				//read the number of previous games

		// average calc: known: previous avg, num of prev. games
		// newAvg = ((prevAvg * numOfPrevGames) + presentScore / numOfPresentGames)
		scoreAvg = ((EEPROM_read16( ADDR_SCORE_AVG ) * (game - 1)) + score) / game;


		EEPROM_write16( ADDR_SCORE_AVG , scoreAvg);


		EEPROM_write( ADDR_GAMENUM , game );

		scoreMin = EEPROM_read16( ADDR_SCORE_MIN );
		scoreMax = EEPROM_read16( ADDR_SCORE_MAX );
	
		if( scoreMin > score)		//if smaller then min, write score as new min
		{
			EEPROM_write16( ADDR_SCORE_MIN, score);
			scoreMin = score;
		}
	
		if( scoreMax < score)		//if greater then max, write score as new max
		{
			EEPROM_write16( ADDR_SCORE_MAX, score);
			scoreMax = score;
		}

		dpy_trm_s01__7seq_clear_dpy();
	
		//statistics display
		while(1){
			if( DPY_TRM_S01__SWITCH_1_GET_STATE() == 1){
			//actual score
				dpy_trm_s01__7seq_write_number(score,0);

			}
			if( DPY_TRM_S01__SWITCH_2_GET_STATE() == 1){
			//avg score
				dpy_trm_s01__7seq_write_number(scoreAvg,0);

			}
			if( DPY_TRM_S01__SWITCH_3_GET_STATE() == 1){
			//best score
				dpy_trm_s01__7seq_write_number(scoreMax,0);
			
			}
			if( DPY_TRM_S01__SWITCH_4_GET_STATE() == 1){
			//worst score
				dpy_trm_s01__7seq_write_number(scoreMin,0);

			}
			if( DPY_TRM_S01__BUTTON_2_GET_STATE() == 0){
			//restart game
				round = 1;
				break;
			}
			for(int i;i<1000;i++){
				
			}

		}

	}

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

