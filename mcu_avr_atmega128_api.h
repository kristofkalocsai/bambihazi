#ifndef _MCU_AVR_ATMEGA128_API_H_
#define _MCU_AVR_ATMEGA128_API_H_

//including standard modules
#include <avr/io.h> //register defines
#include <avr/interrupt.h>
#include <stdio.h>			//FILE access for UART1
#define F_CPU 8000000UL		//CPU clock 8 MHz
#include <util/delay.h>		//for software delays


/*********************************************************************/
/*********************************************************************/
/*************    System  definitinons    ****************************/
/*********************************************************************/
/*********************************************************************/

/*** Version controll ***/

#define MCU_AVR_ATMEGA_128
#define MCU_AVR_ATMEGA_128_VERSION				1			
#define MCU_AVR_ATMEGA_128_MAJOR_VERSION		1
#define MCU_AVR_ATMEGA_128_MINOR_VERSION		1
#define MCU_AVR_ATMEGA_128_SUBMINOR_VERSION		0
#define WINAVR_USED 20060421

/*** Global Mimot PIN implementations ***/

#define SYSTEM_PWM1		1	//ATmega128 TIMER1 OC1A
#define SYSTEM_PWM2		1   //ATmega128 TIMER1 OC1B 
#define SYSTEM_TCAP1	1	//ATmega128 TIMER1
#define SYSTEM_TCAP2	1   //ATmega128 TIMER3
#define SYSTEM_USART	1	//ATmega128L UART1
#define SYSTEM_I2C		1
#define SYSTEM_SPI		1	//SCK shared with ISP_PSCK
#define SYSTEM_DAC		0
#define SYSTEM_ANREF	1
#define SYSTEM_ADC1		1
#define SYSTEM_ADC2		1
#define SYSTEM_ADC3		1
#define SYSTEM_ADC4		1
#define SYSTEM_GPIO_0	1
#define SYSTEM_GPIO_1	1
#define SYSTEM_GPIO_2	1
#define SYSTEM_GPIO_3	1
#define SYSTEM_GPIO_4	1
#define SYSTEM_GPIO_5	1
#define SYSTEM_GPIO_6	1
#define SYSTEM_GPIO_7	1
#define SYSTEM_GPIO_8	1
#define SYSTEM_GPIO_9	1
#define SYSTEM_GPIO_10	1
#define SYSTEM_GPIO_11	1
#define SYSTEM_GPIO_12	1	
#define SYSTEM_GPIO_13	1	
#define SYSTEM_GPIO_14	1	
#define SYSTEM_GPIO_15	1
#define SYSTEM_EXTIT1	1
#define SYSTEM_EXTIT2	1
#define SYSTEM_RESET	1	// not software driveable


/*********************************************************************/
/*********************************************************************/
/******************         General Macros       	  ****************/
/*********************************************************************/
/*********************************************************************/
extern volatile char api_break;

/*********************************************************************
 * Function: 	loop_until_flag_is_set(sfr, bit)
 *
 * Overview:      Waits for the bit in register sfr to be set, or for api_break
  ********************************************************************/
#define loop_until_flag_is_set(sfr, bit) do { } while (bit_is_clear(sfr, bit) && !api_break)



/*********************************************************************
 * Function: 	loop_until_flag_is_clear(sfr, bit)
 *
 * Overview:      Waits for the bit in register sfr to be cleared, or for api_break
  ********************************************************************/
#define loop_until_flag_is_clear(sfr, bit) do { } while (bit_is_set(sfr, bit) && !api_break)


/*********************************************************************/
/*********************************************************************/
/******************         GPIO Pins   		      ****************/
/*********************************************************************/
/*********************************************************************/

//defines for adressing GPIO pins with 16 bit word
//lower byte relates to PORTA, upper byte relates to PORTC
#define GPIO_0  0x0001 //PA0
#define GPIO_1  0x0002 //PA1
#define GPIO_2  0x0004 //PA2
#define GPIO_3  0x0008 //PA3
#define GPIO_4  0x0010 //PA4
#define GPIO_5  0x0020 //PA5
#define GPIO_6  0x0040 //PA6
#define GPIO_7  0x0080 //PA7
#define GPIO_8  0x0100 //PC0
#define GPIO_9  0x0200 //PC1
#define GPIO_10 0x0400 //PC2
#define GPIO_11 0x0800 //PC3
#define GPIO_12 0x1000 //PC4
#define GPIO_13 0x2000 //PC5
#define GPIO_14 0x4000 //PC6
#define GPIO_15 0x8000 //PC7



/*********************************************************************
 * Function: 	GPIO_X_DIR_OUTPUT()
 *
 * PreCondition: 
 *
 * Input:         none
 *
 * Output:        none
 *
 * Side Effects:  Sets the GPIO X direction to output
 *
 * Overview:      Sets the value for the appropriate pin in DDRA/DDRC
 *                register
 *
 * Note:
  ********************************************************************/

#define GPIO_0_DIR_OUTPUT() DDRA|= 0x01
#define GPIO_1_DIR_OUTPUT() DDRA|= 0x02
#define GPIO_2_DIR_OUTPUT() DDRA|= 0x04
#define GPIO_3_DIR_OUTPUT() DDRA|= 0x08
#define GPIO_4_DIR_OUTPUT() DDRA|= 0x10
#define GPIO_5_DIR_OUTPUT() DDRA|= 0x20
#define GPIO_6_DIR_OUTPUT() DDRA|= 0x40
#define GPIO_7_DIR_OUTPUT() DDRA|= 0x80

#define GPIO_8_DIR_OUTPUT() DDRC|= 0x01
#define GPIO_9_DIR_OUTPUT() DDRC|= 0x02
#define GPIO_10_DIR_OUTPUT() DDRC|= 0x04
#define GPIO_11_DIR_OUTPUT() DDRC|= 0x08
#define GPIO_12_DIR_OUTPUT() DDRC|= 0x10
#define GPIO_13_DIR_OUTPUT() DDRC|= 0x20
#define GPIO_14_DIR_OUTPUT() DDRC|= 0x40
#define GPIO_15_DIR_OUTPUT() DDRC|= 0x80


/*********************************************************************
 * Function: 	IO_OUT(GPIO_n)
 *
 * PreCondition: 
 *
 * Input:         GPIO pin mask
 *
 * Output:        none
 *
 * Side Effects:  Sets the appropriate GPIO pin direction to output
 *
 * Overview:      Sets the value for the appropriate pin in DDRA/DDRC
 *                register
 *
 * Note:		  To set multiple pins simultaniously, use '|' operator
  ********************************************************************/
#define	IO_OUT(x)  DDRA|=(char)x; DDRC|=(char)(x>>8)


/*********************************************************************
 * Function: 	GPIO_X_DIR_INPUT()
 *
 * PreCondition: 
 *
 * Input:         none
 *
 * Output:        none
 *
 * Side Effects:  Sets the GPIO X direction to input
 *
 * Overview:      Clears the value for the appropriate pin in DDRA/DDRC
 *                register
 *
 * Note:
  ********************************************************************/
#define GPIO_0_DIR_INPUT() DDRA&= (~0x01)
#define GPIO_1_DIR_INPUT() DDRA&= (~0x02)
#define GPIO_2_DIR_INPUT() DDRA&= (~0x04)
#define GPIO_3_DIR_INPUT() DDRA&= (~0x08)
#define GPIO_4_DIR_INPUT() DDRA&= (~0x10)
#define GPIO_5_DIR_INPUT() DDRA&= (~0x20)
#define GPIO_6_DIR_INPUT() DDRA&= (~0x40)
#define GPIO_7_DIR_INPUT() DDRA&= (~0x80)

#define GPIO_8_DIR_INPUT() DDRC&= (~0x01)
#define GPIO_9_DIR_INPUT() DDRC&= (~0x02)
#define GPIO_10_DIR_INPUT() DDRC&= (~0x04)
#define GPIO_11_DIR_INPUT() DDRC&= (~0x08)
#define GPIO_12_DIR_INPUT() DDRC&= (~0x10)
#define GPIO_13_DIR_INPUT() DDRC&= (~0x20)
#define GPIO_14_DIR_INPUT() DDRC&= (~0x40)
#define GPIO_15_DIR_INPUT() DDRC&= (~0x80)

/*********************************************************************
 * Function: 	IO_IN(GPIO_n)
 *
 * PreCondition: 
 *
 * Input:         GPIO pin mask
 *
 * Output:        none
 *
 * Side Effects:  Sets the appropriate GPIO pin direction to input
 *
 * Overview:      Clears the value for the appropriate pin in DDRA/DDRC
 *                register
 *
 * Note:		  To set multiple pins simultaniously, use '|' operator
  ********************************************************************/
#define	IO_IN(x)   DDRA&=(char)(~x); DDRC&=(char)((~x)>>8)

/*********************************************************************
 * Function: 	GPIO_X_SET()
 *
 * PreCondition: 	Appropriate pins must be defined as output
 *
 * Input:         none
 *
 * Output:        none
 *
 * Side Effects:  Sets the GPIO X to logic 1
 *
 * Overview:      Sets the value for the appropriate pin in PORTA/PORTC
 *                register
 *
 * Note:
  ********************************************************************/

#define GPIO_0_SET() PORTA|= 0x01
#define GPIO_1_SET() PORTA|= 0x02
#define GPIO_2_SET() PORTA|= 0x04
#define GPIO_3_SET() PORTA|= 0x08
#define GPIO_4_SET() PORTA|= 0x10
#define GPIO_5_SET() PORTA|= 0x20
#define GPIO_6_SET() PORTA|= 0x40
#define GPIO_7_SET() PORTA|= 0x80

#define GPIO_8_SET() PORTC|= 0x01
#define GPIO_9_SET() PORTC|= 0x02
#define GPIO_10_SET() PORTC|= 0x04
#define GPIO_11_SET() PORTC|= 0x08
#define GPIO_12_SET() PORTC|= 0x10
#define GPIO_13_SET() PORTC|= 0x20
#define GPIO_14_SET() PORTC|= 0x40
#define GPIO_15_SET() PORTC|= 0x80

/*********************************************************************
 * Function: 	IO_SET(GPIO_n)
 *
 * PreCondition: Appropriate pins must be defined as output
 *
 * Input:         GPIO pin mask
 *
 * Output:        none
 *
 * Side Effects:  Sets the appropriate GPIO pin to logic 1
 *
 * Overview:      Sets the value for the appropriate pin in PORTA/PORTC
 *                register
 *
 * Note:		  To set multiple pins simultaniously, use '|' operator
  ********************************************************************/
#define	IO_SET(x)  PORTA|=(char)x; PORTC|=(char)(x>>8)

/*********************************************************************
 * Function: 	GPIO_X_CLEAR()
 *
 * PreCondition: Appropriate pins must be defined as output
 *
 * Input:         none
 *
 * Output:        none
 *
 * Side Effects:  Sets the GPIO X to logic 0
 *
 * Overview:      Sets the value for the appropriate pin in PORTA/PORTC
 *                register
 *
 * Note:
  ********************************************************************/
#define GPIO_0_CLEAR() PORTA&= (~0x01)
#define GPIO_1_CLEAR() PORTA&= (~0x02)
#define GPIO_2_CLEAR() PORTA&= (~0x04)
#define GPIO_3_CLEAR() PORTA&= (~0x08)
#define GPIO_4_CLEAR() PORTA&= (~0x10)
#define GPIO_5_CLEAR() PORTA&= (~0x20)
#define GPIO_6_CLEAR() PORTA&= (~0x40)
#define GPIO_7_CLEAR() PORTA&= (~0x80)

#define GPIO_8_CLEAR() PORTC&= (~0x01)
#define GPIO_9_CLEAR() PORTC&= (~0x02)
#define GPIO_10_CLEAR() PORTC&= (~0x04)
#define GPIO_11_CLEAR() PORTC&= (~0x08)
#define GPIO_12_CLEAR() PORTC&= (~0x10)
#define GPIO_13_CLEAR() PORTC&= (~0x20)
#define GPIO_14_CLEAR() PORTC&= (~0x40)
#define GPIO_15_CLEAR() PORTC&= (~0x80)

/*********************************************************************
 * Function: 	IO_CLR(GPIO_n)
 *
 * PreCondition: Appropriate pins must be defined as output
 *
 * Input:         GPIO pin mask
 *
 * Output:        none
 *
 * Side Effects:  Sets the appropriate GPIO pin to logic 0
 *
 * Overview:      Clears the value for the appropriate pin in PORTA/PORTC
 *                register
 *
 * Note:		  To set multiple pins simultaniously, use '|' operator
  ********************************************************************/
#define	IO_CLR(x)  PORTA&=(char)(~x); PORTC&=(char)((~x)>>8)

/*********************************************************************
 * Function: 	GPIO_X_GET_VALUE()
 *
 * PreCondition:
 * 
 * Input:         none
 *
 * Output:        The GPIO X pin value: 0 or 1
 *
 * Side Effects:  Gets the GPIO X current value
 *
 * Overview:      Reads the PINA/PINC and AND it with the appropriate pin
 *                register
 *
 * Note:          The output is not 0 or 1
 *
  ********************************************************************/

#define GPIO_0_GET_VALUE() (PINA & 0x01)
#define GPIO_1_GET_VALUE() ((PINA & 0x02) >> 1)
#define GPIO_2_GET_VALUE() ((PINA & 0x04) >> 2)
#define GPIO_3_GET_VALUE() ((PINA & 0x08) >> 3)
#define GPIO_4_GET_VALUE() ((PINA & 0x10) >> 4)
#define GPIO_5_GET_VALUE() ((PINA & 0x20) >> 5)
#define GPIO_6_GET_VALUE() ((PINA & 0x40) >> 6)
#define GPIO_7_GET_VALUE() ((PINA & 0x80) >> 7)

#define GPIO_8_GET_VALUE() (PINC & 0x01)
#define GPIO_9_GET_VALUE() ((PINC & 0x02) >> 1)
#define GPIO_10_GET_VALUE() ((PINC & 0x04) >> 2)
#define GPIO_11_GET_VALUE() ((PINC & 0x08) >> 3)
#define GPIO_12_GET_VALUE() ((PINC & 0x10) >> 4)
#define GPIO_13_GET_VALUE() ((PINC & 0x20) >> 5)
#define GPIO_14_GET_VALUE() ((PINC & 0x40) >> 6)
#define GPIO_15_GET_VALUE() ((PINC & 0x80) >> 7)

/*********************************************************************
 * Function: 	IO_ISSET(GPIO_n)
 *
 * PreCondition: 
 *
 * Input:         GPIO pin mask
 *
 * Output:        Not 0, if the appropriate pin is set
 *
 * Side Effects:  
 *
 * Overview:      Reads PINA/PINC registers
 *
 * Note:		 
  ********************************************************************/
#define IO_ISSET(x)((PINA&(char)x)|(PINC&(char)(x>>8)))


/*********************************************************************/
/*********************************************************************/
/******************         System LED	              ****************/
/*********************************************************************/
/*********************************************************************/

/*********************************************************************
 * Function: 	SYS_LED_DIR_OUTPUT()
 *
 * PreCondition: 
 *
 * Input:         none
 *
 * Output:        none
 *
 * Side Effects:  Sets the SYS_LED pin direction to output
 *
 * Overview:      Sets the value for the appropriate pin in DDRD
 *                register
 *
 * Note:
  ********************************************************************/

#define SYS_LED_DIR_OUTPUT() (DDRD|=0x80)


/*********************************************************************
 * Function: 	SYS_LED_OFF()
 *
 * PreCondition: 	
 *			- The SYS_LED pin direction is specified to output
 *
 * Input:         none
 *
 * Output:        none  
 *
 * Side Effects:  Sets the System LED OFF  
 *
 * Overview:      Sets the value for the appropriate pin in PORTD
 *                register
 *
 * Note:          
  ********************************************************************/

#define SYS_LED_OFF() (PORTD &= 0x7F)


/*********************************************************************
 * Function: 	SYS_LED_ON()        
 *
 * PreCondition: 
 *			- The SYS_LED pin direction is specified to output
 *     
 * Input:         none  
 *
 * Output:        none  
 *
 * Side Effects:  Sets the System LED ON   
 *
 * Overview:      Sets the value for the appropriate pin in PORTD
 *                register
 *
 * Note:          
  ********************************************************************/

#define SYS_LED_ON() (PORTD |= 0x80)


/*********************************************************************
 * Function: 	SYS_LED_GET_VALUE()        
 *
 * PreCondition:  
 *     
 * Input:         none  
 *
 * Output:        0 or 1
 *
 * Side Effects:  none  
 *
 * Overview:      Reads and ANDs the PIND register, then shifts 
 *			the result 
 *
 * Note:          none
  ********************************************************************/

#define SYS_LED_GET_VALUE() ((PIND & 0x80) >> 7)






/*********************************************************************/
/*********************************************************************/
/******************  UART1 (diagnostic serial port)   ****************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************
 * Function: 	UART1_put        
 *
 * PreCondition:  UART1_Init must be called
 *     
 * Input:         char c 
 * 					8 bit binary data to send   
 *
 * Output:        always 0 - required for stdio compatibility
 *
 * Side Effects:  
 *
 * Overview:      Waiting until UDR1 is empty
 * 				  Loading c in UDR1
 *
 * Note:          
  ********************************************************************/
int UART1_put(char c);
//FILE *f input used by fdevopen
int UART1_putf(char c, FILE *f);

/*********************************************************************
 * Function: 	UART1_get        
 *
 * PreCondition:  UART1_Init must be called
 *     
 * Input:          
 *
 * Output:        Data read from UART1
 *
 * Side Effects:  
 *
 * Overview:      Waiting until a byte is received
 * 				  Returning UDR1
 *
 * Note:          
  ********************************************************************/
char UART1_get(void);	  //get without echo


/*********************************************************************
 * Function: 	UART1_get_echo        
 *
 * PreCondition:  UART1_Init must be called
 *     
 * Input:          
 *
 * Output:        Data read from UART1
 *
 * Side Effects:  
 *
 * Overview:      Waiting until a byte is received
 *				  Sending UDR1 (echo)
 * 				  Returning UDR1
 *
 * Note:          
  ********************************************************************/
int UART1_get_echo(FILE *f); //stdio handler uses this one


/*********************************************************************
 * Function: 	UART1_Init        
 *
 * PreCondition:  
 *     
 * Input:         unsigned long baud_rate: 
 * 					Baud rate in bps (other parameters: 8N1)  
 *
 * 				  void (*handler)(void)
 * 					Pointer to function for handling UART1 receive
 * 					if NULL, RX IT is disabled	
 * Output:        none
 *
 * Side Effects:  Global Interrupt enabled
 * 				  Receive IT disabled or linked to function 'handler'  
 *
 * Overview:      Sets up the registers UBRR1H,UBRR1L,UCSR1B, UCSR1C
 * 				  Sets up the FILE handler UART1_handler
 *
 * Note:          stdio functions can be used with UART1_handler
 * 				  writing function: int UART1_put(char c);
 * 				  reading function: UART1_get_echo
  ********************************************************************/
extern FILE *UART1_handler;		//device handler for stdio - set in UART1_Init
void UART1_Init(unsigned long baud_rate, void (*handler)(void));

/*********************************************************************/
/*********************************************************************/
/******************         SPI interface		      ****************/
/*********************************************************************/
/*********************************************************************/

/*********************************************************************
 * Function: 		SPI_Init
 *
 * PreCondition: 	
 *
 * Input:         	unsigned char cpol:
 *							When 1, the SPI clock is active low,
 *							when 0, the SPI clock is active high
 * 					unsigned char cpha;
 *							when 1, the data is sampled on trailing (last) edge of SCK
 * 							when 0, the data is sampled on leading (first) edge of SCK
 *					unsigned char lsbf:
 *							when 1, SPI data is transfered LSB first
 *							when 0, SPI data is transfered MSB first
 *
 *					unsigned char speed:
 *							This value controls the SPI clock rate as fosc/(2^speed)
 *   						Accepted values:
 * 								1 - 7: SPI clock rate as fosc/(2^speed)
 * Output:        	none
 *
 * Side Effects:  	Set the SPI port speed, Clock polarity and phase, and data format
 *
 * Overview:      	Setting up registers DDRB, SPCR, SPSR
 *
 * Note:          	The Slave Select signal need to be controlled by
 * 				  	the user
  ********************************************************************/

void SPI_Init(unsigned char cpol, unsigned char cpha,unsigned char lsbf, unsigned char speed);


/*********************************************************************
 * Function: 		SPI_Slave_Init
 *
 * PreCondition: 	
 *
 * Input:
 * 				  void (*handler)(void)
 * 					Pointer to function for handling SPI receive
 * 					if NULL, SPI IT is disabled	
 * Output:        	none
 *
 * Side Effects:  Set up SPI in slave mode. Map IT function, if given	
 *
 * Overview:      	Setting up registers DDRB, SPCR
 *
 * Note:          	NOT YET  TESTED //TODO
  ********************************************************************/
void SPI_SlaveInit(void (*handler)(void));

/*********************************************************************
 * Function: 		unsigned char SPI_byte(unsigned char data_write, unsigned char* data_read)
 *
 * PreCondition: 	inicialise the SPI port
 *
 * Input:         	unsigned char data_write: the one byte data to be sent
 *  				unsigned char* data_read: buffer for the readed value;
 *
 * Output:        	unsigned char : Error signaling - not yet implemented
 *
 * Side Effects:  	Send and receive one byte through the SPI port
 *
 * Overview:      	Copy the data to the SPDR data register, than
 * 					wait for the SPSR(SPIF) bit to be set, and read the received
 * 					data from the SPDR data register
 *
 * Note:          	The Slave Select signal need to be controlled by
 * 				  	the user
  ********************************************************************/
unsigned char SPI_byte(unsigned char data_write, unsigned char* data_read);



/*********************************************************************/
/*********************************************************************/
/******************         I2C interface		      ****************/
/*********************************************************************/
/*********************************************************************/

/*  Error Codes */
#define	I2C_NOERROR				0
#define	I2C_ERROR				1

/*********************************************************************
 * Function: 		void I2C_init (unsigned int fr)
 *
 * PreCondition: 	
 *
 * Input:         	unsigned int fr :
 *					Selecting the I2c bus speed in kHz. 
 *
 * Output:        	none
 *
 * Side Effects:  	Inicialise and enable the I2C interface in Master mode
 *
 * Overview:      	Setting the I2C clock frequency:  
 * 					fr=F_CPU/1000L/(16+2*TWBR*4^TWPS) TWPS=0,
 * 					TWBR=(F_CPU/1000L/fr - 16)/2 
 *					Setting Up TWCR 					
 *
 * Note:          	documented as TWI interface
 * 
 * 					TWBR>=10, TWBR<=255, F_CPU=8000 kHz ==> 
 * 					fr MUST be between 16..222 kHz
 *  
  ********************************************************************/

void I2C_init (int fr);

/*********************************************************************
 * Function: 		unsigned char I2C_start_write (unsigned char addr)		
 *
 * PreCondition: 	
 * 					- The call of I2C_init () that initializes 
 *			  		the I2C interface
 *     
 * Input:         	addr : The 7bit address of the slave device
 *
 * Output:        	error code: 
 * 							I2C_NOERROR	= 0
 * 							I2C_ERROR	= 1 
 *
 * Side Effects:  	Sends out a Start condition and the Slave address 
 * 					with write flag to the I2C bus. If gets no acknowledge
 * 					error code is returned.
 *
 * Overview:      	Sends a Start condition by setting the TWSTA,TWINT,TWEN bits in 
 * 					TWCR register, then	waits for the TWINT flag,
 * 					If no ack returns an error code is given back, 
 * 					if ack returns then writes out the slave address with wr flag 
 * 					by setting the TWDR register,
 * 					and clearing the TWINT bit to start the transfer.
 * 					After the address trasmission the same ack waiting
 * 					procedure is used as in the start condition case.    
 *
 * Note:          	none
 *  
  ********************************************************************/

unsigned char I2C_start_write (unsigned char addr);


/*********************************************************************
 * Function: 		unsigned char I2C_write (unsigned char data)		
 *
 * PreCondition: 
 * 					- The call of I2C_init () that initializes 
 *			  		the I2C interface
 * 					- The call of I2C_start_write(addr) that sends out 
 * 					a slave address with write condition
 *     
 * Input:         	data : Data to send
 *
 * Output:        	error code: 
 * 							I2C_NOERROR	= 0
 * 							I2C_ERROR	= 1 
 *
 * Side Effects:  	Sends out 8bit data to the I2C bus. If no acknowledge
 * 					returns error code is returned.        
 *
 * Overview:      	At first copies the data into the TWDR register
 *	 				then waits for the TWINT flag, 
 * 					If no ack	returns an error code is returned instead.
 *
 * Note:          	none
 *  
  ********************************************************************/

unsigned char I2C_write (unsigned char data);


/*********************************************************************
 * Function: 		unsigned char I2C_start_read (unsigned char addr)		
 *
 * PreCondition: 	 
 * 					- The call of I2C_init () that initializes 
 *			  		the I2C interface
 *     
 * Input:         	addr : The 7bit address of the slave device
 *
 * Output:        	error code: 
 * 							I2C_NOERROR	= 0
 * 							I2C_ERROR	= 1 
 *
 * Side Effects:  	Sends out a Start condition, and the Slave address 
 * 					with read flag to the I2C bus. If no acknoledge
 * 					returns error code is returned.        
 *
 * Overview:      	Sends a Start condition by setting the TWSTA,TWINT,TWEN bits in 
 * 					TWCR register, then	waits for the TWINT flag,
 * 					If no ack returns an error code is given back, 
 * 					if ack returns then writes out the slave address with wr flag 
 * 					by setting the TWDR register,
 * 					and clearing the TWINT bit to start the transfer.
 * 					After the address trasmission the same ack waiting
 * 					procedure is used as in the start condition case.    
 *
 * Note:          	none
 *  
  ********************************************************************/

unsigned char I2C_start_read (unsigned char addr);


/*********************************************************************
 * Function: 		unsigned char I2C_read (unsigned char *data, 
 * 											unsigned char last_byte)		
 *
 * PreCondition: 	 
 * 					- The call of I2C_init () that initialize 
 *			  		the I2C interface
 * 					- The call of I2C_start_read(addr) that send out 
 * 					a slave address with read condition
 *     
 * Input:         	last_byte : if 1 this is the last byte to read
 * 								therefore no ack is send after receiveing it
 * 
 * 								if 0 there are other bytes to receive 
 * 								therefore ack is send after receiving a byte 
 *
 * Output:        	*data : The returned data
 *  
 * 					error code: 
 * 							I2C_NOERROR	= 0
 * 							I2C_ERROR	= 1 
 *
 * Side Effects:  	Read 8bit data from the I2C bus. If last_byte is true 
 * 					no acknoledge is send, else ack is send.        
 *
 * Overview:      	First set or clear the TWEA bit in the TWCR
 * 					register depended on the last_byte parameter.
 * 					Second clear the TWINT flag in TWCR register
 * 					to start transmittion, then waiting for the TWINT flag,
 * 					If no byte to receive return an error code else fill data and 
 * 					return noerror.     
 *
 * Note:          	none
 *  
  ********************************************************************/


unsigned char I2C_read (unsigned char *data, unsigned char last_byte);



/*********************************************************************
 * Function: 		void I2C_stop (void)
 *
 * PreCondition: 	 
 * 					- The call of I2C_init () that initialize 
 *			  		the I2C interface
 * 					- The start of some kind of I2C communication
 *     
 * Input:         	none
 *
 * Output:        	none
 *
 * Side Effects:  	Send out a Stop condition to the I2C bus
 *
 * Overview:      	Setting the TWSTO, TWINT, TWEN flag in TWCR 
 * 					to send out the stop condition.
 * 					 
 * Note:          	none
 *  
  ********************************************************************/

void I2C_stop (void);

#endif //_MCU_AVR_ATMEGA128_API_H_
