#include "config_peripheriques.h"

#ifndef config_global
#define config_global
#include "config_global.h"
#endif

/*************** variables globales *****************/
extern unsigned char Intensite;
extern unsigned char Lum_ON;
extern unsigned char Lum_OFF;
extern unsigned char Lum_Nbre;
extern unsigned int cpt;
extern unsigned int nbOverflow;
extern unsigned char ON;
/**************************************************/


/*********************** Timer 3 *************************************************************/
void configTimer3(void)
{
	EIE2 |= 0x01; //Active interruption timer3 (Mise à 1 du bit 0 du registre EIE2: permet d'activer l'interruption du timer3.)
	TMR3L = 0x00;
	TMR3H = 0x00;//Initialisation du timer3 à FFFF (Mise à 1 de l'enssemble des 8 bits des registres TMR3L et TMR3H.)
	TMR3RLL = 0xFF;// permet de générer une interruption toutes les 25ms
	TMR3RLH = 0x4B;// permet de générer une interruption toutes les 25ms
	TMR3CN = 0x04;
}
void ISRtimer3(void) interrupt 14
{
	int pourcentage;
	cpt++;
	if(cpt==nbOverflow)
	{
		if(ON==1){
			nbOverflow=Lum_OFF*4;
			ON=0;
			PCA0CPL4 = 0xFF;//pourcentage%256; // Permet d'avoir un rapport cyclique de Intensité 0%
			PCA0CPH4 = 0xFF;//pourcentage/256; // Permet d'avoir un rapport cyclique de Intensité 0%*/
		}
		else{
			nbOverflow=Lum_ON*4;
			ON=1;
			if(Lum_Nbre!=1){
			pourcentage = 65535 -655*Intensite ; // obtenu avec excel + wizzard
			// régler PCA à pourcentage
			PCA0CPL4 = pourcentage%256; // Permet d'avoir un rapport cyclique de Intensité%
			PCA0CPH4 = pourcentage/256; // Permet d'avoir un rapport cyclique de Intensité%
			}
		}
		Lum_Nbre--;
		cpt=0;
	}
	TMR3CN&=0x7F; // on remet le flag à zero
}

/*******************************************************************************************/


/******************************* PCA (module 4) *******************************************/

void PCA_Init(void)
{
PCA0MD = 0x88; // suspend PCA opeartion while system controller is in Idle Mode & enable interrupt
PCA0CPM4 = 0xC2; // Module4 in 16-bit Pulse Width Modulator
PCA0L = 0x00; // Permet d'avoir une fréquence de 65536*sysclk/12
PCA0H = 0x00; // Permet d'avoir une fréquence de 65536*sysclk/12
PCA0CPL4 = 0xFF; // Permet d'avoir un rapport cyclique de 0% => par défaut la LED doit être éteinte
PCA0CPH4 = 0xFF; // Permet d'avoir un rapport cyclique de 0%
PCA0CN = 0x40; // enable pca
}


/********************************************************************************************/



/***************************** Laser ****************************************************/

void Lumiere (void)
{
Lum_Nbre*=2;
configTimer3();
while(Lum_Nbre>0)
{
}
TMR3CN=0; // stop le timer 3
}
/*
Extinction inconditionnelle de la source lumineuse
*/
void Lumiere_Stop (void)
{
TMR3CN=0;// stop le timer 3
PCA0CPL4 = 0xFF;//pourcentage%256; // Permet d'avoir un rapport cyclique de Intensité 0%
PCA0CPH4 = 0xFF;//pourcentage/256; // Permet d'avoir un rapport cyclique de Intensité 0%*/
}

/**********************************************************************************************/
