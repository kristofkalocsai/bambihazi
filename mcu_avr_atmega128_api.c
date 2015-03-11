#include "mcu_avr_atmega128_api.h"
volatile char api_break=0;
/*********************************************************************/
/*********************************************************************/
/******************  UART1 (diagnostic serial port)   ****************/
/*********************************************************************/
/*********************************************************************/
void (*UART1_IT_Handler)(void)=0; //pointer to IT handler function - set in UART1_Init
ISR(USART1_RX_vect) {UART1_IT_Handler();}

FILE *UART1_handler;		//device handler for stdio - set in UART1_Init

void UART1_Init(unsigned long baud_rate, void (*handler)(void)) 
{
	cli();
// Set baud rate 
	unsigned int baud_set_value=(unsigned int)((F_CPU/(16*baud_rate)-1) & 0x0fff);
	UBRR1H = (unsigned char)(baud_set_value>>8);
	UBRR1L = (unsigned char)baud_set_value;

	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10); 			 // 8N1
	UCSR1B = (1<<RXEN1)|(1<<TXEN1); 			 // transmit, recieve
	if(handler) {UCSR1B|=(1<< RXCIE1); UART1_IT_Handler=handler;}		 // recieve IT
	if(UART1_handler){fclose(UART1_handler); UART1_handler=0;}		//if already opened (reinit): close dev. handler 
	UART1_handler=fdevopen(&UART1_putf,&UART1_get_echo);				//open device handler
	sei();
}

int UART1_put(char c)
{
	 loop_until_flag_is_set(UCSR1A, UDRE);
     UDR1 = c;
     return 0;
}

int UART1_putf(char c, FILE *f) {return UART1_put(c);}

char UART1_get(void)
{
	 loop_until_flag_is_set(UCSR1A, RXC);
     return UDR1;
}

int UART1_get_echo(FILE* f)
{
	int i=UART1_get();
	if(i=='\r'){ UART1_putf(i,f); i='\n';} //gets cuts \n
	UART1_putf(i,f);
	return i;
}


/*********************************************************************/
/*********************************************************************/
/******************         SPI interface		      ****************/
/*********************************************************************/
/*********************************************************************/
void (*SPI_IT_Handler)(void)=NULL; //pointer to IT handler function - set in UART1_Init
SIGNAL(SIG_SPI) {SPI_IT_Handler();}


void SPI_Init(unsigned char cpol, unsigned char cpha,unsigned char lsbf, unsigned char speed)
{
	DDRB|=0x07; DDRB&=0xf7;  //MOSI SCK SS output, MISO input 
	SPCR = _BV(SPE)|_BV(MSTR) | (cpol<<CPOL) | (cpha<<CPHA) | (lsbf<<DORD);
	if(speed==3 || speed==4 || speed==7) SPCR|= _BV(SPR0);
	if(speed>=5) SPCR|= _BV(SPR1);		
	if(speed==1 || speed==3 || speed==5) SPSR|=_BV(SPI2X);		
}


void SPI_SlaveInit(void (*handler)(void)) //argument: interrupt handler function
{
	DDRB|=0x08; DDRB&=0xf8;   
	SPCR = _BV(SPE);
	if(handler) {SPI_IT_Handler=handler; SPCR |= _BV(SPIE);}
}

unsigned char SPI_byte(unsigned char data_write, unsigned char* data_read)
{
	SPDR=data_write;
//	printf("API > SPI sending\r\n");
	loop_until_flag_is_set(SPSR,SPIF);
	if(data_read) *data_read=SPDR;
//	printf("API > SPI done\r\n");
	return 0; //SPI_NOERROR; //TODO
}

/*********************************************************************/
/*********************************************************************/
/******************         I2C interface		      ****************/
/*********************************************************************/
/*********************************************************************/




void I2C_init (int fr)
{
	//Setting Bitrate
	TWSR=0;
	TWBR=((int)(F_CPU/1000L/fr)-16)/2;
	TWCR=_BV(TWEN);
}

unsigned char I2C_start(unsigned char addr, unsigned char rnw) //internal
{
	int i;

	//IT table: 215., 219. p.
	addr<<=1;
	if(rnw) addr|=1;							//Set RW flag
	unsigned char state;

	
	for(i=0; i<1000 && !api_break; ++i)		//try the starting more times
	{
		TWCR=_BV(TWINT)|_BV(TWSTA)|_BV(TWEN);		//Send START condition
		loop_until_flag_is_set(TWCR, TWINT);		//wait for TWINT = START transmitted
		state=TWSR & 0xf8;
		if(state!=0x08 && state!=0x10) continue;	//if no (repeated) START transmitted

		TWDR=addr;								//load address	
		TWCR=_BV(TWINT)|_BV(TWEN);					//Clear IT flag, start transmission
		loop_until_flag_is_set(TWCR, TWINT);			//wait for TWINT = addr transmitted, acked
		state=TWSR & 0xf8;
		if(    ( (!rnw) && (state==0x18)) ||				//if no SLA+W ACK in write mode and
			   (  rnw   && (state==0x40)))				//no SLA+R ACK in read mode
					return I2C_NOERROR;
	}
	return I2C_ERROR;

}

unsigned char I2C_start_write (unsigned char addr)
{
	return I2C_start(addr,0);
}

unsigned char I2C_write (unsigned char data)
{
	TWDR=data;
	TWCR=_BV(TWINT)|_BV(TWEN);					//Clear IT flag, start transmission
	loop_until_flag_is_set(TWCR, TWINT);		//wait for TWINT = data transmitted, acked
	if((TWSR & 0xf8)!=0x28) 
	{
//		printf("api> I2C write error!\r\n");
		return I2C_ERROR;	//if no data ACK received
	}
	return I2C_NOERROR;
}

unsigned char I2C_start_read (unsigned char addr)
{
	return I2C_start(addr,1);	
}

unsigned char I2C_read (unsigned char *data, unsigned char last_byte)
{
	if(last_byte) //No ACK
		TWCR = _BV(TWINT) | _BV(TWEN);
	else
		TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	loop_until_flag_is_set(TWCR, TWINT);			//wait for TWINT = data received
	if(api_break) return I2C_ERROR;	
	*data=TWDR;									//read data
	return I2C_NOERROR;
}

void I2C_stop (void)
{
	TWCR=_BV(TWINT)|_BV(TWSTO)|_BV(TWEN);		//Send STOP condition
}
