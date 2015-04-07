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
unsigned char temps;


//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
void main (void) {
	Intensite=80;
	Lum_ON=50;// choisir valeur entre 1 et 100
	Lum_OFF=40;// choisir valeur entre 1 et 100 !!!! attention prendre différent de Lum_ON
	Lum_Nbre=10;
	temps=Lum_ON*10*12/SYSCLK;
	
	Init_Device();
	PCA_Init();
	EA=1; // Autorisation interruptions

	Lumiere();
	while (1) { // Boucle infinie
	
	}
}
