//------------------------------------------------------------------------------------
// main.c
//------------------------------------------------------------------------------------
// AUTH: Groupe A1
// DATE: 10/03/2015
//
// Target: C8051F020
// Tool chain: KEIL Eval 'c'
//
//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------//
#ifndef config_global
#define config_global
#include "config_global.h"
#endif	// Configuration globales du µprocesseur

#include "config_peripheriques.h"


#ifndef structure
#define structure
#include "structure.h"
#endif

#include <stdio.h>
//------------------------------------------------------------------------------------
// Global CONSTANTS
//------------------------------------------------------------------------------------
unsigned char Intensite;
unsigned char Lum_ON;
unsigned char Lum_OFF;
unsigned char Lum_Nbre;
unsigned int cpt;
unsigned int nbOverflow;
unsigned char ON;

//sbit LED = P1^6;

		
/* pour la spi: ***********************************************/
bit transmit_completed= 0;

char tableau[20]; // stock chaine treçue SPI
int index = 0; // index sur tableau

int fin_spi=0;
/******************** fonctions SPI *********************/

// Reçoit des chaines de la forme "DD" + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + "FF"
int Reception_Trame_SPI(char str[])
{
	
	cmd_c.Etat_Lumiere = str[2];
	cmd_c.Lumiere_Intensite = str[3];
	cmd_c.Lumiere_Duree = str[4];
	cmd_c.Lumiere_Extinction = str[5];
	cmd_c.Lumiere_Nbre = str[6];
	cmd_c.Etat_ACQ_Son = str[7];
	cmd_c.ACQ_Duree = str[8];
	return 1;
}


//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
void main (void) {	

	/* initialisation des variables globales*/
	
	/* pour le laser : *************************************/
	/*Intensite=80; // en %
	Lum_ON=20;// choisir valeur entre 1 et 100
	Lum_OFF=10;// choisir valeur entre 1 et 100
	Lum_Nbre=10;*/
	//char xdata machaine[50];
	int xdata test;
	//sprintf(machaine,"DD%c%c%c%c%c%c%cFF",0x01,0x10,0x01,0x10,0x04,0x00,0x00);
	//test = Reception_Trame_SPI(machaine);
	
	/****************************************************/
	Init_Device();
	PCA_Init();
	SPI_Init();
	
	while (1) { // Boucle infinie
		
		if(fin_spi==1)
		{
			test=Reception_Trame_SPI(tableau);
			index=0;
			fin_spi=0;
		}
		
		if(cmd_c.Etat_Lumiere==Allumer)
		{
			Intensite=cmd_c.Lumiere_Intensite;
			Lum_ON=cmd_c.Lumiere_Duree;
			Lum_OFF=cmd_c.Lumiere_Extinction;
			Lum_Nbre=cmd_c.Lumiere_Nbre+1;
			
			cpt=0;
			ON=1;
			nbOverflow=Lum_ON*4; // (x*10^-1)/(25*10^-3) rappel: un overflow tt les 25 ms
			Lumiere();
			
			cmd_c.Etat_Lumiere=Eteindre;
		}
		
		if(cmd_c.Etat_Lumiere==Eteindre)
		{
			Lumiere_Stop ();
		}
	}
}
