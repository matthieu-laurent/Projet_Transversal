//------------------------------------------------------------------------------------
// Projet BSE 2014 - Fichier configuration globale du 8051F020
//------------------------------------------------------------------------------------
//
// AUTH: FARAMOND Victor & CHENE Charly
// DATE: 06-10-2014
// Target: C8051F02x
//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------

#include <LIB_BSE_Config_Globale.h>
void Reset_Sources_Init()
{
WDTCN = 0xDE; // Désactive le timer du watchdog
WDTCN = 0xAD;
}
void Oscillator_Init()
{
int i = 0; // Compteur délai
OSCXCN = 0x67; // Démarrage oscillateur externe avec crystal à 22,1184 Mhz
for (i = 0; i < 3000; i++); // Wait 1ms for initialization
while ((OSCXCN & 0x80) == 0); // Attente de la mise en place de l'oscillateur crystal
OSCICN = 0x0C; // Sélection de l'oscillateur externe comme source pour SYSCLK et activation du détecteur de clock manquante
}
// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
Reset_Sources_Init(); // Désactivation du watchdog
Oscillator_Init(); // SYSCLOCK = 22,1184 Mhz
}
