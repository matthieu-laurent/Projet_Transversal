
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
CFG_Clock_UART1();
CFG_UART1();
while(1)
{
Action_UART1();
Avancer(5);// pour le test
}
}
