/* @section  I N C L U D E S */
#include <C8051F020.h>
#include <string.h>
#include <stdio.h>
#include "FO_M1__Structures_COMMANDES_INFORMATIONS_CentraleDeCommande.h"
char serial_data;
char data_example=0x55;
char data_save;
bit transmit_completed= 0;
sbit nss2slave = P0^4;
char* p;
int fin = 0;
int i;
char c;

char* Envoi_Trame_SPI()
{
	char machaine[15];
	sprintf(machaine, "DD%c%c%c%c%c%c%cFF\0", cmd_c.Etat_Lumiere, cmd_c.Lumiere_Intensite, cmd_c.Lumiere_Duree, cmd_c.Lumiere_Extinction, cmd_c.Lumiere_Nbre, cmd_c.Etat_ACQ_Son, cmd_c.ACQ_Duree);
	
	return machaine;
}

// Renvoie 1 si trame ok, 0 sinon
// Reçoit des chaines de la forme "DD" + 0x00 + 0x01 + "FF"
int Reception_Trame_SPI(char* str)
{
	char *p = str;

	if(*p != 'D')	return 0;
	if(*p != '\0') p++;
	if(*p != 'D') return 0;
	if(*p != '\0') p++;
	cmd_c.Etat_Lumiere = *p;
	if(*p != '\0') p++;
	cmd_c.Etat_ACQ_Son = *p;
	if(*p != '\0') p++;
	if(*p != 'F') return 0;
	if(*p != '\0') p++;
	if(*p != 'F') return 0;
	return 1;
}

void Reset_Sources_Init()
{
    WDTCN     = 0xDE;
    WDTCN     = 0xAD;
}

void SPI_Init()
{
    SPI0CFG   = 0x87;
    SPI0CN    = 0x03;
	  SPI0CKR   = 0x04;
}

void Port_IO_Init()
{
    // P0.0  -  SCK  (SPI0), Open-Drain, Digital
    // P0.1  -  MISO (SPI0), Open-Drain, Digital
    // P0.2  -  MOSI (SPI0), Open-Drain, Digital
    // P0.3  -  NSS  (SPI0), Open-Drain, Digital
    // P0.4  -  Unassigned,  Open-Drain, Digital
    // P0.5  -  Unassigned,  Open-Drain, Digital
    // P0.6  -  Unassigned,  Open-Drain, Digital
    // P0.7  -  Unassigned,  Open-Drain, Digital

    // P1.0  -  Unassigned,  Open-Drain, Digital
    // P1.1  -  Unassigned,  Open-Drain, Digital
    // P1.2  -  Unassigned,  Open-Drain, Digital
    // P1.3  -  Unassigned,  Open-Drain, Digital
    // P1.4  -  Unassigned,  Open-Drain, Digital
    // P1.5  -  Unassigned,  Open-Drain, Digital
    // P1.6  -  Unassigned,  Open-Drain, Digital
    // P1.7  -  Unassigned,  Open-Drain, Digital

    // P2.0  -  Unassigned,  Open-Drain, Digital
    // P2.1  -  Unassigned,  Open-Drain, Digital
    // P2.2  -  Unassigned,  Open-Drain, Digital
    // P2.3  -  Unassigned,  Open-Drain, Digital
    // P2.4  -  Unassigned,  Open-Drain, Digital
    // P2.5  -  Unassigned,  Open-Drain, Digital
    // P2.6  -  Unassigned,  Open-Drain, Digital
    // P2.7  -  Unassigned,  Open-Drain, Digital

    // P3.0  -  Unassigned,  Open-Drain, Digital
    // P3.1  -  Unassigned,  Open-Drain, Digital
    // P3.2  -  Unassigned,  Open-Drain, Digital
    // P3.3  -  Unassigned,  Open-Drain, Digital
    // P3.4  -  Unassigned,  Open-Drain, Digital
    // P3.5  -  Unassigned,  Open-Drain, Digital
    // P3.6  -  Unassigned,  Open-Drain, Digital
    // P3.7  -  Unassigned,  Open-Drain, Digital

		P0MDOUT   = 0x1D;
    XBR0      = 0x02;
    XBR2      = 0x40;
}

void Oscillator_Init()
{
		int i = 0;
		OSCXCN    = 0x67;// crystal oscillator mode + f> 6, 7MHz + osc. not yet stable
		for (i = 0; i < 3000; i++);  // Wait 1ms for initialization
		while ((OSCXCN & 0x80) == 0); // on attend que l'oscillateur soit stable
		OSCICN    = 0x0C; // utilisation de l'oscillateur externe

}

void Interrupts_Init()
{
    IE        = 0x80;
    EIE1      = 0x01;
		EA = 1;
}

// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
    Reset_Sources_Init();
    SPI_Init();
    Port_IO_Init();
    Oscillator_Init();
    Interrupts_Init();
}

/**
 * FUNCTION_PURPOSE: This file set up spi in master mode with 
 * Fclk Periph/128 as baud rate and with slave select pin.
 * FUNCTION_INPUTS: P1.5(MISO) serial input
 * FUNCTION_OUTPUTS: P1.7(MOSI) serial output
 */
 
 
void main(void)
{
	Init_Device();
	
	p = Envoi_Trame_SPI();
	while(*p != '\0')
	{
		c = *p;
		p++;
	}
	fin = 0;
	
	while(*p != '\0' && fin != 1)
	{
		nss2slave = 0;
		SPI0DAT = *p;
		while(!transmit_completed);
		transmit_completed = 0;
		p++;
	}

	while(1)			   			/* endless  */
   {
		 nss2slave = 0; // Met NSS à 1
	   SPI0DAT=data_example;        /* send an example data */
		 data_example++;
	   while(!transmit_completed);/* wait end of transmition */
		 transmit_completed = 0;
		 p++;
 
   }

}


/**
 * FUNCTION_PURPOSE:interrupt
 * FUNCTION_INPUTS: void
 * FUNCTION_OUTPUTS: transmit_complete is software transfert flag
 */
void it_SPI(void) interrupt 6 
{  
	nss2slave=1; // Met NSS à 1
	while(TXBSY);
	//serial_data=SPI0DAT;   /* read receive data */
	transmit_completed = 1;
	SPIF = 0;
}


