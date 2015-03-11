/*********************************************************************
 *
 *              Application Programming Interface (API) 
 *                     for DPY-TRM-S-01a Module
 *
 *********************************************************************
 * FileName:        dpy_trm_s01.c
 * Dependencies:    dpy_trm_s01.h
 * Processor:       processor independent
 * Complier:        compiler independent              
 * Company:         Budapest University of Technology and Economics
 * 					Department of Measurement and Information Systems
 * 
 * note:
 *	General API for the DPY-TRM-S-01a Mitmót module. This API use the
 * 	Mitmót BUS functions specified by the mcu API.
 *
 * Author               Date    Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Balazs Scherer     1/27/05   Original
 ********************************************************************/
 

#include "dpy_trm_s01.h"




/*********************************************************************
 * Function: 	  void dpy_trm_s01__Init(void)
 *
 * PreCondition:  The inicialisation of the mcu API
 *     
 * Input:         none  
 *
 * Output:        none  
 *
 * Side Effects:  Inicialize the LEDs, and The Buttons, the Switch pin 
 * 				  dirrection and value. Set the SPI Chip select to High
 * 				  Inicialize the Temperature sensor.  
 *					
 * Overview:      Call the appropirate GPIO_x_DIR_OUTPUT, GPIO_x_DIR_INPUT
 * 				  functions and Clear the display by calling dpy_trm_s01__7seq_clear_dpy(). 
 * 				  And call the dpy_trm_s01__Temp_Init(); 
 *
 * Note:          Uses mcu common BUS functions.
  ********************************************************************/

void dpy_trm_s01__Init(void)
{
	/* Pulling inactive the 7 segment display chip select */
	GPIO_0_DIR_OUTPUT();
	GPIO_0_SET();
	
	/* Setting the Button pins directions */
	GPIO_1_DIR_INPUT();
	GPIO_2_DIR_INPUT();
	GPIO_3_DIR_INPUT();

	/* Setting the LED pins directions */
	GPIO_4_DIR_OUTPUT();
	GPIO_5_DIR_OUTPUT();
	GPIO_6_DIR_OUTPUT();
	GPIO_7_DIR_OUTPUT();
	
	/* Setting the Switch pins directions */
	GPIO_8_DIR_INPUT();
	GPIO_9_DIR_INPUT();
	GPIO_10_DIR_INPUT();
	GPIO_11_DIR_INPUT();

#if ARM_LPC2106	
	I2C_init (100,100);
	SPI_Init(0,0,0,20);
//	printf("dpy arm init done \r\n");
	
#elif AVR_ATMEGA128
	SPI_Init(0,0,0,4);
	I2C_init(50);
//	printf("dpy avr init done \r\n");

#endif
	
	dpy_trm_s01__7seq_clear_dpy();
	dpy_trm_s01__Temp_Init(TMP75_JUMPER_OFF,TMP75_JUMPER_OFF,TMP75_JUMPER_OFF);
}


/*********************************************************************/
/*********************************************************************/
/**********          Temperature Sensor			      ****************/
/*********************************************************************/
/*********************************************************************/

#define TMP75_BASE_ADDRESS 0x48

/*********************************************************************
 * Function: 	  void dpy_trm_s01__Temp_Init(
 * 								unsigned char adr_jp1,
 * 								unsigned char adr_jp2, 
 * 								unsigned char adr_jp3)
 *
 * PreCondition:  The inicialisation of the mcu API
 *     
 * Input:         unsigned char adr_jp1  : The state of the address jumper 1 (Top)
 * 				  unsigned char adr_jp2  : The state of the address jumper 2 
 * 				  unsigned char adr_jp3  : The state of the address jumper 3 
 * 
 * 					The jumper state should be TMP75_JUMPER_OFF or TMP75_JUMPER_ON
 *
 * Output:        Error code : 
 * 							DPY_TRM_S01_TEMP__NOERROR    	0
 *		 					DPY_TRM_S01_TEMP__ERROR			1
 *
 * Side Effects:  Inicialise the TMP75 to 12 bit data precision and set the
 * 				  base pointer to temperature read.		
 *					
 * Overview:      First I2C communication is to the configuration register
 * 				  selecting 12bit resoulution, The seconds sets the pointer
 * 				  to the Temperature read register 
 *
 * Note:          Uses mcu common BUS functions.
 * 				  Called by dpy_trm_s01__Init
  ********************************************************************/

unsigned char dpy_trm_s01__Temp_Init(unsigned char adr_jp1, unsigned char adr_jp2, unsigned char adr_jp3)
{
unsigned char address, error;
  
  address = (TMP75_BASE_ADDRESS | adr_jp1 | (adr_jp2<<1) | (adr_jp2<<2));

/* Configurating the TMP75 to 12 bit resolution */	
  error = I2C_start_write (address);
  if (error == I2C_ERROR)
  		return DPY_TRM_S01_TEMP__ERROR;
  error = I2C_write (0x01);
  if (error == I2C_ERROR)
  		return DPY_TRM_S01_TEMP__ERROR;
  error = I2C_write (0x60);
  if (error == I2C_ERROR)
  		return DPY_TRM_S01_TEMP__ERROR;
  I2C_stop ();
//  printf("tempinit 1st phase ok\r\n");		
  

/* Setting the pointer to the Temperature read register */
  
  error = I2C_start_write (address);
  if (error == I2C_ERROR)
  		return DPY_TRM_S01_TEMP__ERROR;
  error = I2C_write (0x00);
  if (error == I2C_ERROR)
  		return DPY_TRM_S01_TEMP__ERROR;
  I2C_stop ();
//  printf("tempinit 2nd phase ok\r\n");		
  
	return DPY_TRM_S01_TEMP__NOERROR;	
}

/*********************************************************************
 * Function: 	  unsigned char  dpy_trm_s01__Temp_ReadTEMP(
 * 								float *temp,
 * 								unsigned char adr_jp1,
 * 								unsigned char adr_jp2, 
 * 								unsigned char adr_jp3)
 *
 * PreCondition:  The inicialisation of the mcu API
 *     
 * Input:         float *temp  :           pointer for temperature storage
 * 				  unsigned char adr_jp1  : The state of the address jumper 1 (Top)
 * 				  unsigned char adr_jp2  : The state of the address jumper 2 
 * 				  unsigned char adr_jp3  : The state of the address jumper 3 
 * 
 * 					The jumper state should be TMP75_JUMPER_OFF or TMP75_JUMPER_ON
 *
 * Output:        Error code : 
 * 							DPY_TRM_S01_TEMP__NOERROR    	0
 *		 					DPY_TRM_S01_TEMP__ERROR			1
 *
 * Side Effects:  Read the temperature from TMP75 	
 *					
 * Overview:      First I2C communication sets the pointer to the Temperature read register,
 * 					the second reads the temperature. The temperature is returned as a float number 
 *
 * Note:          Uses mcu common BUS functions.
  ********************************************************************/

unsigned char dpy_trm_s01__Temp_ReadTEMP(float* temp, unsigned char adr_jp1, unsigned char adr_jp2, unsigned char adr_jp3)
{
unsigned char address, error;
short int  d;
  
/* Calculating the TMP75 address */  
  
  address = (TMP75_BASE_ADDRESS | adr_jp1 | (adr_jp2<<1) | (adr_jp2<<2));


/* Setting the pointer to the Temperature read register */
  
  error = I2C_start_write (address);
  if (error == I2C_ERROR)
  		return DPY_TRM_S01_TEMP__ERROR;
  error = I2C_write (0x00);
  if (error == I2C_ERROR)
  		return DPY_TRM_S01_TEMP__ERROR;
  I2C_stop ();
//  printf("tempread 1st phase ok\r\n");		



/* Reading the Temperature */

   error = I2C_start_read (address);
   if (error == I2C_ERROR)
  		return DPY_TRM_S01_TEMP__ERROR;
   error = I2C_read ((unsigned char*)&d+1,0);
   if (error == I2C_ERROR)
  		return DPY_TRM_S01_TEMP__ERROR;
   error = I2C_read ( ((unsigned char*)&d),1);
   if (error == I2C_ERROR)
  		return DPY_TRM_S01_TEMP__ERROR;
   I2C_stop ();

//  printf("tempread 2nd phase ok\r\n");		

//   diag_printf("%d %x            ",d,d);
   if (d & 0x8000)
   			d = ((d>>4) | 0xF000);
   		else 
   			d = (d>>4);
//   	 diag_printf("%d %x\r\n",d,d);
 	 *temp =  d * 0.0625;

return DPY_TRM_S01_TEMP__NOERROR;	
}





 
/*********************************************************************/
/*********************************************************************/
/**********          7-segment Display			      ****************/
/*********************************************************************/
/*********************************************************************/
   
 //     0
//   -----
//  |     |
// 4|    1|
//  |  5  |
//   -----
//  |     |  
// 6|    2| ---
//  |  7  || 3 |
//   -----  ---

/* Number Table 0 1 2 3 4 5 6 7 8 9 None - Dot */
const unsigned char seg_numbers[13]={
	0x28, 0xF9, 0x1C,
	0x58, 0xC9, 0x4A,
	0x0A, 0xF8, 0x08,
	0x48, 0xFF, 0xDF, 0xF7};

#define SEG_SPACE 10
#define SEG_MIN	  11
#define SEG_DOT	  12

#define ABS(x) (x>0 ? x : -x)	

/*********************************************************************
 * Function: 	  void dpy_trm_s01__7seq_write_3digit(	unsigned char data1,
 * 														unsigned char data2, 
 * 														unsigned char data3 )
 *
 * PreCondition:  The call of dpy_trm_s01__Init()
 *     
 * Input:         	unsigned char data1 : Data for the left side 7segment
 * 					unsigned char data2 : Data for the center 7segment
 * 					unsigned char data3 : Data for the right 7segment
 *
 * Output:        none  
 *
 * Side Effects:  Dispalys the data on the 7segment dpy.   
 *					
 * Overview:      Uses the chip select and send out the data1-3 using 
 * 				  the SPI_byte(data) function.
 *
 * Note:          Uses mcu common BUS functions.
  ********************************************************************/

void dpy_trm_s01__7seq_write_3digit(unsigned char data1, unsigned char data2, unsigned char data3 )
{
	
/*  Pulling down the chip select */
	GPIO_0_CLEAR();

	SPI_byte(data1,0);
	SPI_byte(data2,0);
	SPI_byte(data3,0);

/*  Pulling up the chip select */	
	GPIO_0_SET();

}


/*********************************************************************
 * Function: 	  void dpy_trm_s01__7seq_clear_dpy(void)
 *
 * PreCondition:  The call of dpy_trm_s01__Init()
 *     
 * Input:         none  
 *
 * Output:        none  
 *
 * Side Effects:  Clears the 7segment dpy.     
 *					
 * Overview:      Calls dpy_trm_s01__7seq_write_3digit(d1,d2,d3) with
 * 				  SEG_SPACE codes
 *
 * Note:          Useable for power saving.
  ********************************************************************/

void dpy_trm_s01__7seq_clear_dpy(void)
{
	dpy_trm_s01__7seq_write_3digit(seg_numbers[SEG_SPACE],seg_numbers[SEG_SPACE],seg_numbers[SEG_SPACE]);
}


/*********************************************************************
 * Function: 	  unsigned char dpy_trm_s01__7seq_write_number(
 * 									float number,
 * 									unsigned char decimal_fraction)
 *
 * PreCondition:  The call of dpy_trm_s01__Init()
 *     
 * Input:         float number :  The float number to display
 * 				  unsigned char decimal_fraction : The number of decimal fractions
 *
 * Output:        Error code : 
 * 							DPY_TRM_S01_7SEG__NOERROR    	0
 *		 					DPY_TRM_S01_7SEG__ERROR			1
 *
 * Side Effects:  Displays the float number on the 7segment dpy.     
 *					
 * Overview:      Check the range of the number determined by decimal_fraction
 * 				  Ranges 
 * 							decimal_fraction 0 : -99 <-> 999
 * 							decimal_fraction 1 : -9.9 <-> 99.9
 * 							decimal_fraction 2 : 0 <-> 9.99
 * 
 *				  Then displays the correct values usnig the 
 * 				  dpy_trm_s01__7seq_write_3digit(d1,d2,d3) function.
 * 				  If the number is not in the range an error code is returned.
 * 
 * Note:          none
  ********************************************************************/

unsigned char dpy_trm_s01__7seq_write_number(float number, unsigned char decimal_fraction)
{

	unsigned char d1,d2,d3;


	switch (decimal_fraction)
		{
			case 0:
			
				if( (number>999) || (number<-99) )
					return DPY_TRM_S01_7SEG__ERROR;
				
				if(number>0)	
					{
					if ( ABS( (number/100) ) >= 1 )
							d1 = number/100;  /* First digit */
					   else
							d1 = SEG_SPACE;			  /* writing space */			    	
					}
				   else
		   				d1 = SEG_MIN;	/* writing - */			    	
					 
				 if ( ABS((number/10)) >= 1 )
						d2 = ABS( (( ((int)number) % 100) / 10) ); /* writing second digit */
							else
						d2 = SEG_SPACE;	/* writing space */			    	

				d3 = ABS(( ((int)number) % 10 ) ); /* writing third digit */
					
				dpy_trm_s01__7seq_write_3digit(seg_numbers[d1],seg_numbers[d2],seg_numbers[d3]);
							
			 break;
			 
			case 1:
				if( (number>=100) || (number<-9) )
					return DPY_TRM_S01_7SEG__ERROR;
					
				if(number>0)	
					{
					if ( ABS( (number/10) ) >= 1 )
							d1 = number/10;  /* First digit */
					   else
							d1 = SEG_SPACE;			  /* writing space */			    	
					}
				   else
		   				d1 = SEG_MIN;	/* writing - */			    	
					 
				d2 = ABS( ( ((int)number) % 10) ); /* writing second digit */
				
				d3 = ( (int)(ABS(number) * 10) % 10); /* writing third digit */
								
				dpy_trm_s01__7seq_write_3digit(seg_numbers[d1],seg_numbers[d2] & seg_numbers[SEG_DOT], seg_numbers[d3]);
							
					
			 break;

			case 2:
				if( (number>=10) || (number<0) )
					return DPY_TRM_S01_7SEG__ERROR;
					
				d1 = number;
				d2 = ( (int)(ABS(number) * 10) % 10);
				d3 = ( (int)(ABS(number) * 100) % 10);
					
				dpy_trm_s01__7seq_write_3digit(seg_numbers[d1] & seg_numbers[SEG_DOT],seg_numbers[d2], seg_numbers[d3]);	
				
			 break;
			 
			default:
				return DPY_TRM_S01_7SEG__ERROR;
			 break;
			
			
		}
	
return DPY_TRM_S01_7SEG__NOERROR;
}


