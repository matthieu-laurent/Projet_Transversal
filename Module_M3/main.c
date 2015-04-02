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
#include "fonctions_servo.h"

#ifndef FO_M1__STRUCTURES_COMMANDES_INFORMATIONS_CENTRALEDECOMMANDE_H
#define FO_M1__STRUCTURES_COMMANDES_INFORMATIONS_CENTRALEDECOMMANDE_H
#include "FO_M1__Structures_COMMANDES_INFORMATIONS_CentraleDeCommande.h"
#endif

sbit P14=P1^4;
//------------------------------------------------------------------------------------
// Global CONSTANTS
//------------------------------------------------------------------------------------
/************ ServoMoteur ****************/



//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
void main (void) {
	unsigned long int i=0;
	char Angle=0;
	Init_Device();
	PCA_Init();
	EA=1; // Autorisation interruptions

//	cmd_c.Etat_Servo=Servo_V;
//	cmd_c.Servo_Angle=-90;
	
	while (1) { // Boucle infinie
			CDE_Servo();
			/*for(i=0;i<50000;i++)
			cmd_c.Servo_Angle=cmd_c.Servo_Angle+5;
			if(cmd_c.Servo_Angle==90)
				cmd_c.Servo_Angle=-90;*/
	}
}
