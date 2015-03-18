//------------------------------------------------------------------------------------
// Projet BSE 2014 - Fichier configuration et gestion de l'UART0
//------------------------------------------------------------------------------------
//
// AUTH: FARAMOND Victor & CHENE Charly
// DATE: 06-10-2014
// Target: C8051F02x

//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include "c8051f020.h"                   // SFR declarations
#include "Ports_Declarations_TP_Emb.h"
#include "LIB_BSE_UART.h"
#include "FO_M1__Structures_COMMANDES_INFORMATIONS_CentraleDeCommande.h"
#include <stdlib.h>     // atoi 
#include <stdio.h>   // printf

void CFG_Clock_UART0()
{
	CKCON |= 0x10; // T1M : Timer 1 Clock Select
	
	TMOD |= 0x20; // On met T1M1 à 1
	
	TMOD &= 0x2F; // On met T1M0 à 0
	
	// Timer 1 configuré en timer 8 bits avec auto-reload
	
	TF1 = 0; // On met à 0 le bit d'overflow du Timer 1
	
	TH1 = -(SYSCLK / BAUDRATE / 16);
	
	ET1 = 1; // Interruption Timer 1 désactivée
	
	TR1 = 1; // Timer 1 démarré


}

void CFG_UART0()
{
	RCLK0 = 0; // Bit 5 de T2CON : source clk Timer 1
	
	TCLK0 = 0; // Bit 4 de T2CON : transmit clk flag Timer 1
	
	PCON |= 0x80; // On met à 1 SMOD0 pour désactiver le doubleur de baudrate
	
	PCON &= 0x0BF; // On met à 0 SSTAT0
	
	SCON0 = 0x72; // Mode 1 - Check Stop Bit - Réception validée - Transmission - Octet transmis (prêt à recevoir un char pour transmettre)
	//SCON0 = 0x50;
}

char Putchar(char c, char csg_tempo)
{
	int tempo = (int)csg_tempo;
	int i = 0;
	
	while (!TI0)
	{
		for(i=0 ; i<443 ; i++); // Temporisation 20us
		
		tempo--;
		
		if (tempo == 0) return 0;
		
	}
	
	SBUF0 = c;
	
	TI0=0;
	
	return c;
}

char Send_String(char *char_ptr)
{
	int i = 0;
	char ret = 0;
	int erreur = 0;
	char cpt = 0;
	
	while(char_ptr[i] != '\0')
	{
		ret = Putchar(char_ptr[i],10);
		
		i++;
		
		if(ret == 0) erreur = 1;
		if(ret != 0) cpt++;
	}
	
	switch(erreur)
	{
		case 0 : return cpt;
		
		case 1 : return 0;
		
		default : return 0;
	}
}

char Getchar()
{
	char car = 0;
	
	if(RI0 == 0) return car;
		
	car = SBUF0;
	
	RI0 = 0;
	
	return car;
}

void Demo_UART()
{
	int i;
	char c= 'a';
	char tempo = 'a';
	char str[20] = "Bonjour";
	
	Putchar(c,tempo);
	
	for(i=0;i<1000;i++);
	
	c = 'b';
	
	Putchar(c,tempo);
	
	for(i=0;i<1000;i++);
	
	c = 'c';
	
	Putchar(c,tempo);
	
	for(i=0;i<1000;i++);
	
	Send_String(str);
	
	for(i=0;i<1000;i++);
	TI0 = 0;
	
	c = Getchar();
	
}

void Analyse_String(char* str)
{
	char *p = str;
	
	while(*p != '\r')
	{
		switch (*p)
		{
			case 'D':
				p++;
				p++;
				switch (*p)
				{
					case '1':
						out.Etat_Epreuve = epreuve1;
						break;
					case '2':
						out.Etat_Epreuve = epreuve2;
						break;
					case '3':
						out.Etat_Epreuve = epreuve3;
						break;
					case '4':
						out.Etat_Epreuve = epreuve4;
						break;
					case '5':
						out.Etat_Epreuve = epreuve5;
						break;
					case '6':
						out.Etat_Epreuve = epreuve6;
						break;
					case '8':
						out.Etat_Epreuve = epreuve8;
						break;
					default:
						out.Etat_Epreuve = Epreuve_non;
						break;
				}
				break;
			case 'E' :
				out.Etat_Epreuve = Fin_Epreuve;
				break;
			case 'Q' :
				out.Etat_Epreuve = Stop_Urgence;
				break;
			case 'T' :
				p++;
				if(*p == 'V')
				{
					char vitesse[4];
					p++;
					if(*p >= '0' && *p <= '9')
					{
						vitesse[0] = *p;
					}
					else
					{
						break;
					}
					p++;
					if(*p >= '0' && *p <= '9')
					{
						vitesse[1] = *p;
					}
					else
					{
						break;
					}
					p++;
					if(*p >= '0' && *p <= '9') // Vitesse à 3 chiffres
					{
						vitesse[2] = *p;
						vitesse[3] = '\0';
						if(atoi(vitesse) == 100) out.Vitesse = vitesse; 
						break;
					}
					else // Vitesse à 2 chiffres
					{
						vitesse[2] = '\0';
						if(atoi(vitesse) < 100 && atoi(vitesse) >= 10) out.Vitesse = vitesse;
						break;						
					}
				}
				else
				{
					break;
				}
			case 'A':
				break;
					
		}
	}
}
