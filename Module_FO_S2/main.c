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
#endif				// Configuration globales du µprocesseur
#include "init_pca.h"
#include "laser.h"
#include "configTimer3Slave.h"

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

//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
void main (void) {

	
	Intensite=80; // en %
	Lum_ON=20;// choisir valeur entre 1 et 100
	Lum_OFF=10;// choisir valeur entre 1 et 100 
	Lum_Nbre=10; 
	cpt=0;
	ON=1;
	nbOverflow=Lum_ON*4; // (x*10^-1)/(25*10^-3) rappel: un overflow tt les 25 ms
	
	Init_Device();
	PCA_Init();
	EA=1; // Autorisation interruptions
	
	Lumiere();
	while (1) { // Boucle infinie
	
	}
}
