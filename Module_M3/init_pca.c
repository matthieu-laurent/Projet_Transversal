#include <init_pca.h>

void PCA_Init(void)
{
		CF 				= 0; // remise à zéro du flag
    EIE1 			+= 0x08; // enable pca interrupt
    PCA0MD    = 0x81; // suspend PCA opeartion while system controller is in Idle Mode & enable interrupt
		PCA0L     = 0xFF;
    PCA0H     = 0x00; // Permet d'avoir une fréquence de 50Hz
    // Pour le module  0
		PCA0CPM0  = 0xC2; // Module0 in 16-bit Pulse Width Modulator
    PCA0CPL0  = 0xCC;
    PCA0CPH0  = 0xCC; // Permet d'avoir un rapport cyclique de 20%
		// Pour le module 1
	  PCA0CPM1  = 0xC2; // Module1 in 16-bit Pulse Width Modulator
    PCA0CPL1  = 0xCC; 
    PCA0CPH1  = 0xCC; // Permet d'avoir un rapport cyclique de 20%
		// Pour le module 2
	  PCA0CPM2  = 0xC2; // Module1 in 16-bit Pulse Width Modulator
    PCA0CPL2  = 0xCC;
    PCA0CPH2  = 0xCC; // Permet d'avoir un rapport cyclique de 20%
		// Pour le module 3

		// Pour le module 4
	
		PCA0CN    = 0x40; // enable PCA
}

void PCA_ISR(void) interrupt 9
{
	PCA0L     = 0x07;
  PCA0H     = 0x70; // Permet d'avoir une fréquence de 50Hz
	CF 				= 0; // remise à zéro du flag
}
