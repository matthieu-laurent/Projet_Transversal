#include "init_pca.h"

void PCA_Init(void)
{
    PCA0MD    = 0x88; // suspend PCA opeartion while system controller is in Idle Mode & enable interrupt
    PCA0CPM4  = 0xC2; // Module4 in 16-bit Pulse Width Modulator
    
    PCA0L     = 0x00; // Permet d'avoir une fréquence de 65536*sysclk/12
		PCA0H     = 0x00; // Permet d'avoir une fréquence de 65536*sysclk/12
    
		PCA0CPL4  = 0xFF; // Permet d'avoir un rapport cyclique de 0% => par défaut la LED doit être éteinte 
    PCA0CPH4  = 0xFF; // Permet d'avoir un rapport cyclique de 0%
    
		PCA0CN    = 0x40; // enable pca
}




