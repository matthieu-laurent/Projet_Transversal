#include "configTimer3Slave.h"

#ifndef config_global
#define config_global
#include "config_global.h"
#endif	



extern unsigned char Intensite;
extern unsigned char Lum_ON;
extern unsigned char Lum_OFF;
extern unsigned char Lum_Nbre;
extern unsigned char temps;

void configTimer3(void)
{
	TMR3CN = 0x00;       //Desactiver timer3 (Permet d'initialiser à 0 le registre de control du timer 3, celui ci est alors désactivé afin de ne pas commencer à compter avant les configurations)                   
	EIE2   |= 0x01;      //Active interruption timer3 (Mise à 1 du bit 0 du registre EIE2: permet d'activer l'interruption du timer3.)                   
	TMR3H    = 0xff;
	TMR3L    = 0xff;    //Initialisation du timer3 à FFFF (Mise à 1 de l'enssemble des 8 bits des registres TMR3L et TMR3H.)
	TMR3CN |= 0x04;      //Activer timer3 (Permet d'activer le timer 3 en mettant à 1 le bit 3 du registre TMR3CN).
}

void ISRtimer3(void) interrupt 14
{
	int pourcentage;
	TMR3CN = 0x00;       //desactivation timer

	TMR3H  = 255-temps/256;   // Le registre TMR3RL contient la valeur de  rechargement. Lorsque le timer a atteint l'overflow, celui recommence à compter à partir de cette valeur de rechargement.  (l'opération effectuée est FFFF-count)                  
	TMR3L  = 255-temps%256;

	if(temps==Lum_ON*10*12/SYSCLK)
	{
		temps=Lum_OFF*10*12/SYSCLK;
		pourcentage = -655,35*Intensite + 65535; // obtenu avec excel + wizzard
		// régler PCA à pourcentage
		PCA0CPL0  = 255-pourcentage%256; // Permet d'avoir un rapport cyclique de Intensité% 
    PCA0CPH0  = 255-pourcentage/256; // Permet d'avoir un rapport cyclique de Intensité%
	}
	else
	{
		temps=Lum_ON*12/SYSCLK; 
		PCA0CPL0  = 0xFF; // Permet d'avoir un rapport cyclique de 0% 
    PCA0CPH0  = 0xFF; // Permet d'avoir un rapport cyclique de 0%
	}
	
	Lum_Nbre--;
	TMR3CN |= 0x04;      //Permet d'activer le timer 3 en mettant à 1 le bit 3 du registre TMR3CN.
}
