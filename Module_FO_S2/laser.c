#include "laser.h"

#ifndef config_global
#define config_global
#include "config_global.h"
#endif	

#ifndef configTimer3Slave
#define configTimer3Slave
#include "configTimer3Slave.h"
#endif
/*
Intensité: intensité lumineuse exprimée en pourcentage de luminosité

Lum_ON: Durée d’allumage exprimée en dixièmes de seconde (de 1 à 100)

Lum_OFF: Durée d’extinction exprimée en dixièmes de seconde (de 1 à 100)

Lum_OFF: Nombre de cycles d’allumage-Extinction (de 1 à 100) 
*/

/* Variables globales*/
	extern unsigned char Intensite;
	extern unsigned char Lum_ON;
	extern unsigned char Lum_OFF;
	extern unsigned char Lum_Nbre;
	extern unsigned int cpt;
	extern unsigned int nbOverflow;
/******************************/

void Lumiere (void) //unsigned char Intensite, unsigned char Lum_ON, unsigned char Lum_OFF, unsigned char Lum_Nbre) 
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
	PCA0CPL4  = 0xFF;//pourcentage%256; // Permet d'avoir un rapport cyclique de Intensité 0% 
	PCA0CPH4  = 0xFF;//pourcentage/256; // Permet d'avoir un rapport cyclique de Intensité 0%*/
}
