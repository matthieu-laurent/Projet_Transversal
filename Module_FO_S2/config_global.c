#include <config_global.h>

void Init_Device(void)
{
	gestion_reset();
	gestion_watchdog();
	gestion_horloge();
	gestion_memoires();
	gestion_puissance();
	gestion_brocheIO();
}

void gestion_reset(void)
{
}

void gestion_watchdog(void)
{
// Utilisation du registre WDTCN
	EA=0;//désactivation des interruptions car les deux lignes suivantes doivent être exécutées à la suite.
	WDTCN=0xDE;// désactivation du watchdog
	WDTCN=0xAD;// désactivation du watchdog
	EA=1;//activation des interruptions
}

void gestion_horloge(void)
{
	//utilisation des registres OSCXCN et OSCICN
	int i = 0;
	OSCXCN    = 0x67;// crystal oscillator mode + f> 6, 7MHz + osc. not yet stable
	for (i = 0; i < 3000; i++);  // Wait 1ms for initialization
	while ((OSCXCN & 0x80) == 0); // on attend que l'oscillateur soit stable
	OSCICN    = 0x0C; // utilisation de l'oscillateur externe
}

void gestion_memoires(void)
{
}

void gestion_puissance(void)
{
}

void gestion_brocheIO(void)
{
		// mappage du signal issue du module 4 du pca sur p1.6
		P1MDOUT   = 0x40;
    XBR0      = 0x2F;
    XBR2      = 0x44;


	
}
