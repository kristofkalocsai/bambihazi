/*********************************************************************
 *
 *              Application Programming Interface (API) 
 *                     for DPY-TRM-S-01a Module
 *
 *********************************************************************
 * FileName:        dpy_trm_s01.h
 * Dependencies:    some kind of mcu card header file
 * 					for exaple mcu_arm_s01.h
 * Processor:       processor independent
 * Complier:        compiler independent              
 * Company:         Budapest University of Technology and Economics
 * 					Department of Measurement and Information Systems
 * 
 * note:
 *	General API for the DPY-TRM-S-01a Mitmot module. This API use the
 * 	Mitmote BUS functions specified by the mcu API.
 *
 * Author               Date    Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Balazs Scherer     1/27/05   Original
 ********************************************************************/
 
#ifndef _DPY_TRM_S01_H_
#define _DPY_TRM_S01_H_
#define _DPY_TRM_S01_VERSION 1.0.0.0

#include "platform.h"


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

void dpy_trm_s01__Init(void);




/*********************************************************************/
/*********************************************************************/
/**********          LED controll				      ****************/
/*********************************************************************/
/*********************************************************************/




/*********************************************************************
 * Function: 	DPY_TRM_S01__LED_x_OFF()        
 *
 * PreCondition:  The call of dpy_trm_s01__Init()
 *     
 * Input:         none  
 *
 * Output:        none  
 *
 * Side Effects:  Set the LED x OFF 
 *
 * Overview:      Call the appropriate GPIO_x_CLEAR()
 *
 * Note:          none
  ********************************************************************/

#define DPY_TRM_S01__LED_1_OFF() GPIO_4_CLEAR()
#define DPY_TRM_S01__LED_2_OFF() GPIO_5_CLEAR()
#define DPY_TRM_S01__LED_3_OFF() GPIO_6_CLEAR()
#define DPY_TRM_S01__LED_4_OFF() GPIO_7_CLEAR()


/*********************************************************************
 * Function: 	DPY_TRM_S01__LED_x_ON()        
 *
 * PreCondition:  The call of dpy_trm_s01__Init()
 *     
 * Input:         none  
 *
 * Output:        none  
 *
 * Side Effects:  Set the LED x ON  
 *
 * Overview:      Call the appropriate GPIO_x_SET()
 *
 * Note:          none
  ********************************************************************/

#define DPY_TRM_S01__LED_1_ON() GPIO_4_SET()
#define DPY_TRM_S01__LED_2_ON() GPIO_5_SET()
#define DPY_TRM_S01__LED_3_ON() GPIO_6_SET()
#define DPY_TRM_S01__LED_4_ON() GPIO_7_SET()


/*********************************************************************
 * Function: 	DPY_TRM_S01__LED_x_GET_STATE()        
 *
 * PreCondition:  The call of dpy_trm_s01__Init()
 *     
 * Input:         none  
 *
 * Output:        The LED x current state   
 *
 * Side Effects:  Get the LED x current state  
 *
 * Overview:      Call the appropriate GPIO_x_GET_VALUE()
 *
 * Note:          none
  ********************************************************************/

#define DPY_TRM_S01__LED_1_GET_STATE() GPIO_4_GET_VALUE()
#define DPY_TRM_S01__LED_2_GET_STATE() GPIO_5_GET_VALUE()
#define DPY_TRM_S01__LED_3_GET_STATE() GPIO_6_GET_VALUE()
#define DPY_TRM_S01__LED_4_GET_STATE() GPIO_7_GET_VALUE()


/*********************************************************************/
/*********************************************************************/
/**********          Switch controll			      ****************/
/*********************************************************************/
/*********************************************************************/

/*********************************************************************
 * Function: 	DPY_TRM_S01__SWITCH_x_GET_STATE()        
 *
 * PreCondition:  The call of dpy_trm_s01__Init()
 *     
 * Input:         none  
 *
 * Output:        The SWITCH X current state  
 *
 * Side Effects:  Get the Switch x current state  
 *
 * Overview:      Call the appropriate GPIO_x_GET_VALUE()
 *
 * Note:          none
  ********************************************************************/

#define DPY_TRM_S01__SWITCH_1_GET_STATE() GPIO_8_GET_VALUE()
#define DPY_TRM_S01__SWITCH_2_GET_STATE() GPIO_9_GET_VALUE()
#define DPY_TRM_S01__SWITCH_3_GET_STATE() GPIO_10_GET_VALUE()
#define DPY_TRM_S01__SWITCH_4_GET_STATE() GPIO_11_GET_VALUE()



/*********************************************************************/
/*********************************************************************/
/**********          Button controll			      ****************/
/*********************************************************************/
/*********************************************************************/

/*********************************************************************
 * Function: 	DPY_TRM_S01__BUTTON_x_GET_STATE()        
 *
 * PreCondition:  The call of dpy_trm_s01__Init()
 *     
 * Input:         none  
 *
 * Output:        The BUTTON X current state  
 *
 * Side Effects:  Get the Button x current state  
 *
 * Overview:      Call the appropriate GPIO_x_GET_VALUE()
 *
 * Note:          none
  ********************************************************************/

#define DPY_TRM_S01__BUTTON_1_GET_STATE() GPIO_1_GET_VALUE()
#define DPY_TRM_S01__BUTTON_2_GET_STATE() GPIO_2_GET_VALUE()
#define DPY_TRM_S01__BUTTON_3_GET_STATE() GPIO_3_GET_VALUE()



/*********************************************************************/
/*********************************************************************/
/**********          Temperature Sensor			      ****************/
/*********************************************************************/
/*********************************************************************/


#define TMP75_JUMPER_OFF	1
#define TMP75_JUMPER_ON		0

#define DPY_TRM_S01_TEMP__NOERROR    	0
#define DPY_TRM_S01_TEMP__ERROR			1


/*********************************************************************
 * Function: 	  unsigned char  dpy_trm_s01__Temp_Init(
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

unsigned char dpy_trm_s01__Temp_Init(unsigned char adr_jp1, unsigned char adr_jp2, unsigned char adr_jp3);

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

unsigned char dpy_trm_s01__Temp_ReadTEMP(float* temp, unsigned char adr_jp1, unsigned char adr_jp2, unsigned char adr_jp3);

 
 
 
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




/* Error codes */
 
#define DPY_TRM_S01_7SEG__NOERROR    	0
#define DPY_TRM_S01_7SEG__ERROR			1


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

void dpy_trm_s01__7seq_write_3digit(unsigned char data1, unsigned char data2, unsigned char data3 );


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

void dpy_trm_s01__7seq_clear_dpy(void);


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

unsigned char dpy_trm_s01__7seq_write_number(float number, unsigned char decimal_fraction);




#endif /*  _DPY_TRM_S01_H_ */









































