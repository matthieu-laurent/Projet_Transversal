#include <init_pca.h>

void PCA_Init(void)
{
    PCA0MD    = 0x81; // suspend PCA opeartion while system controller is in Idle Mode & enable interrupt
    PCA0CPM0  = 0xC2; // Module0 in 16-bit Pulse Width Modulator
    PCA0CPM1  = 0xC2; // Module1 in 16-bit Pulse Width Modulator
    PCA0CPM2  = 0xC2; // Module2 in 16-bit Pulse Width Modulator
    PCA0L     = 0x07; // Permet d'avoir une fréquence de 50Hz
		PCA0H     = 0x70; // Permet d'avoir une fréquence de 50Hz
    
		PCA0CPL0  = 0xFD; // Permet d'initialiser le servo 0 à la position 0°
    PCA0CPH0  = 0xF4; // Permet d'initialiser le servo 0 à la position 0°
		PCA0CPL1  = 0xFD; // Permet d'initialiser le servo 1 à la position 0°
    PCA0CPH1  = 0xF4; // Permet d'initialiser le servo 1 à la position 0°
		PCA0CPL2  = 0xFD; // Permet d'initialiser le servo 2 à la position 0°
		PCA0CPH2  = 0xF4; // Permet d'initialiser le servo 2 à la position 0°
    
		PCA0CN    = 0x40;
    EIE1      = 0x08; // enable pca interrupt
}

void PCA_ISR(void) interrupt 9
{
	PCA0L     = 0x07;
  PCA0H     = 0x70; // Permet d'avoir une fréquence de 50Hz
	CF 				= 0; // remise à zéro du flag
}
