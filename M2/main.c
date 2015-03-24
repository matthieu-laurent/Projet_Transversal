//------------------------------------------------------------------------------------
// Projet BSE 2014 - Fichier main
//------------------------------------------------------------------------------------
//
// AUTH: FARAMOND Victor & CHENE Charly
// DATE: 06-10-2014
// Target: C8051F02x
//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
// SFR declarations

#include <LIB_BSE_Config_Globale.h>
#include <LIB_BSE_GPIO.h>
#include <LIB_BSE_UART.h>


#include <intrins.h>
#include <stdio.h>
#include <stdlib.h>

#define SYSCLK 22118400 //approximate SYSCLK frequency in Hz
#define BAUDRATE 9600 // Baud rate of UART in bps
#define TICK_CLK 200 // (en hertz)


void main()
{
Init_Device();
Config_GPIO();
CFG_Clock_UART1();
CFG_UART1();
while(1)
{
Action_UART1();
Avancer(5);// pour le test
}
}
