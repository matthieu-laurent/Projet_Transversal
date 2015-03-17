#include <init_pca.h>

void PCA_Init()
{
    PCA0CN    = 0x40; // enable PCA
    PCA0MD    = 0x80; // suspend PCA opeartion while system controller is in Idle Mode
    // Pour le module  0
		PCA0CPM0  = 0xC2; // Module0 in 16-bit Pulse Width Modulator
    PCA0L     = 0xFF;
    PCA0H     = 0x6F; // Permet d'avoir une fréquence de 50Hz
    PCA0CPL0  = 0xCC;
    PCA0CPH0  = 0xCC; // Permet d'avoir un rapport cyclique de 20%
		// Pour le module 1
	  PCA0CPM1  = 0xC2; // Module1 in 16-bit Pulse Width Modulator
    PCA0CPL1  = 0xCC; 
    PCA0CPH1  = 0xCC; // Permet d'avoir un rapport cyclique de 20%
		// Pour le module 2
	  PCA0CPM2  = 0xC2; // Module1 in 16-bit Pulse Width Modulator
    PCA0CPL1  = 0xCC;
    PCA0CPH1  = 0xCC; // Permet d'avoir un rapport cyclique de 20%
		// Pour le module 3

		// Pour le module 4
}
