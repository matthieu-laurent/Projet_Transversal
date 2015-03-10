// Module SPI partie Slave

//-----------------------------------------------------------------------------   
// Includes   
//----------------------------------------------------------------------------- 
#include "C8051F020.h"
//-----------------------------------------------------------------------------   
// Global Constants   
//----------------------------------------------------------------------------- 
#define SYSCLK             16000000    // Internal oscillator frequency in Hz 

#define SPI_CLOCK          250000      // Maximum SPI clock   
                                       // The SPI clock is a maximum of 250 kHz   
                                       // when this example is used with   
                                       // the SPI0_Slave code example. 									   
//-----------------------------------------------------------------------------   
// Global Variables   
//-----------------------------------------------------------------------------   
char serial_data;
char data_example = 0x55;
char data_save;
bit transmit_completed = 0;						   
//-----------------------------------------------------------------------------   
// Function Prototypes   
//-----------------------------------------------------------------------------  
void Reset_Sources_Init(void);
void Oscillator_Init(void);
void SPI_Init(void);
void Port_IO_Init(void);
void Interrupts_Init(void);
void Init_Device(void);
void SPI_Byte_Write (void);   
void SPI_Byte_Read (void);   
void SPI_Array_Write (void);   
void SPI_Array_Read (void);
//-----------------------------------------------------------------------------  
// FUNCTION_PURPOSE: This file set up spi in master mode with 
// Fclk Periph/128 as baud rate and with slave select pin.
// FUNCTION_INPUTS: P1.5(MISO) serial input
// FUNCTION_OUTPUTS: P1.7(MOSI) serial output
//----------------------------------------------------------------------------- 
void main(void)
{
	Init_Device();
	EA = 1;
	transmit_completed = 1;
	serial_data = 0x20;

	while(1)			   			/* endless  */
   {
	   if(transmit_completed)
		{
		   SPI0DAT = serial_data;       /* echo data to master */
		   transmit_completed = 0;    /* clear software transfert flag */
		}
   }

}

//-----------------------------------------------------------------------------   
// Initialization Subroutines   
//-----------------------------------------------------------------------------   
   
//-----------------------------------------------------------------------------   
// Reset_Sources_Init  
//-----------------------------------------------------------------------------   
//   
// Return Value : None   
// Parameters   : None   
//   
// This function disables the watchdog timer.   
//   
//-----------------------------------------------------------------------------  
void Reset_Sources_Init()
{
    WDTCN     = 0xDE;
    WDTCN     = 0xAD;
}

//-----------------------------------------------------------------------------   
// Oscillator_Init   
//-----------------------------------------------------------------------------   
//   
// Return Value : None   
// Parameters   : None   
//   
// This function initializes the system clock to use the external crystal oscillator  
// at 12 MHz.   
//   
//-----------------------------------------------------------------------------  
void Oscillator_Init()
{
    int i = 0;
    OSCXCN    = 0x67;
    for (i = 0; i < 3000; i++);  // Wait 1ms for initialization
    while ((OSCXCN & 0x80) == 0);
    OSCICN    = 0x08;
}

//-----------------------------------------------------------------------------   
// SPI_Init   
//-----------------------------------------------------------------------------   
//   
// Return Value : None   
// Parameters   : None   
//   
// Configures SPI0 to use 4-wire Slave mode. The SPI timing is   
// configured for Mode 0,0 (data centered on first edge of clock phase and   
// SCK line low in idle state).   
//   
//----------------------------------------------------------------------------- 
void SPI_Init()
{
    SPI0CN    = 0x01; // 4-wire Slave mode, SPI enabled
}

//-----------------------------------------------------------------------------   
// Port_IO_Init   
//-----------------------------------------------------------------------------   
//   
// Return Value : None   
// Parameters   : None   
//   
// This function configures the crossbar and GPIO ports.   
//   
// P0.0  -  TX0 (UART0), Open-Drain, Digital
// P0.1  -  RX0 (UART0), Open-Drain, Digital
// P0.2  -  SCK  (SPI0), Push-Pull,  Digital
// P0.3  -  MISO (SPI0), Open-Drain, Digital
// P0.4  -  MOSI (SPI0), Push-Pull,  Digital
// P0.5  -  NSS  (SPI0), Push-Pull,  Digital 
//   
//----------------------------------------------------------------------------- 
void Port_IO_Init()
{
	P0MDOUT   = 0x34;
    XBR0      = 0x06;
    XBR2      = 0x40;
}

//-----------------------------------------------------------------------------   
// Interrupts_Init   
//-----------------------------------------------------------------------------   
//   
// Return Value : None   
// Parameters   : None   
//   
// This function enables the SPI0 interruption.
//   
//----------------------------------------------------------------------------
void Interrupts_Init()
{
    IE        = 0x80; // Enable All Interrupts
    EIE1      = 0x01; // Bit ESPI0 activated (vector 6)
}

//-----------------------------------------------------------------------------   
// Init_Device   
//-----------------------------------------------------------------------------   
//   
// Return Value : None   
// Parameters   : None   
//   
// Calls all device initialization functions.   
//   
//-----------------------------------------------------------------------------  
void Init_Device(void)
{
	Reset_Sources_Init();
    SPI_Init();
    Port_IO_Init();
	Oscillator_Init();
    Interrupts_Init();
}

//-----------------------------------------------------------------------------
// FUNCTION_PURPOSE:interrupt
// FUNCTION_INPUTS: void
// FUNCTION_OUTPUTS: transmit_complete is software transfert flag
//-----------------------------------------------------------------------------
void SPI_ISR(void) interrupt 6 // Adress : 0x0033
{
	while(!TXBSY); // On attend la fin 
	
	if(RXOVRN) // Overrun case
	{
		RXOVRN = 0;
		SPIF = 0;
	}
	if(SPIF) // Fin d'un transfert
	{
		serial_data = SPI0DAT;
		transmit_completed = 1;
		SPIF = 0;
	}
	
    SPI0DAT = serial_data;        /* needed to complete clearing sequence */
}
