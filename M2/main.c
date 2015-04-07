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

#include <LIB_BSE_UART.h>


#include <intrins.h>
#include <stdio.h>
#include <stdlib.h>

void main()
{
	unsigned long int i = 0;
	Init_Device();
	CFG_Clock_UART1();
	CFG_UART1();
	Init_Commande_Serializer();

while(1)
{
	Avancer(15);// pour le test
	Action_UART1();
	for(i=0;i<5000;i++);
	Arret();
	Action_UART1();
	for(i=0;i<5000;i++);/*
	Rotation_Droite_90(15);
	Action_UART1();
	for(i=0;i<500000;i++);
	Reculer(10);
	Action_UART1();
	for(i=0;i<500000;i++);
	Arret();
	Action_UART1();
	for(i=0;i<500000;i++);*/
}


}
