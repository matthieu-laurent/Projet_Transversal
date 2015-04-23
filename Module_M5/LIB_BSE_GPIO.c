#include <LIB_BSE_GPIO.h>
#include <LIB_BSE_Config_Global.h>

void Config_GPIO(void)
{
	// P0.0  -  Tx UART0, PushPull
	// P0.1  -  Rx UART0
	// P0.2  -  Unassigned,  Open-Drain, Digital
	// P0.3  -  Unassigned,  Open-Drain, Digital
	// P0.4  -  Unassigned,  Open-Drain, Digital
	// P0.5  -  Unassigned,  Open-Drain, Digital
	// P0.6  -  Unassigned,  Open-Drain, Digital
	// P0.7  -  Unassigned,  Open-Drain, Digital

	// P1.0  -  Tx UART1, PushPull
	// P1.1  -  Rx UART1
	// P1.2  -  CEX0, PushPull
	// P1.3  -  CEX1, PushPull
	// P1.4  -  CEX2, PushPull
	// P1.5  -  Unassigned,  Open-Drain, Digital
	// P1.6  -  LED Verte - NE PAS UTILISER CE PORT POUR AUTRE CHOSE
	// P1.7  -  Unassigned,  Open-Drain, Digital

	// P2.0  -  Decl_PP1,  Push-Pull,  Digital
	// P2.1  -  Decl_PP2,  Push-Pull,  Digital
	// P2.2  -  DETECT,  Open-Drain, Digital
	// P2.3  -  CT1_DCT,  Push-Pull,  Digital
	// P2.4  -  CT2_DCT,  Push-Pull,  Digital
	// P2.5  -  CT3_DCT,  Push-Pull,  Digital
	// P2.6  -  CHG_DCT,  Push-Pull,  Digital
	// P2.7  -  RAZ_CP,  Open-Drain, Digital

	// P3.0  -  RAZ_RTC,  Open-Drain, Digital
	// P3.1  -  START_Sys,  Open-Drain, Digital
	// P3.2  -  SIG_erreur,  Push-Pull,  Digital
	// P3.3  -  Decl_PES,  Push-Pull,  Digital
	// P3.4  -  Trig UltraSon
	// P3.5  -  Echo Ultrason
	// P3.6  -  Trig UltraSon_AR
	// P3.7  -  Echo Ultrason_AR

		P1MDOUT   = 0x1D;
    P3MDOUT   = 0x50;
   /* XBR0      = 0xEF;
    XBR1      = 0xFF;
    XBR2      = 0x5D;*/
		
		//test moteur
		//XBR0      = 0x07;
		 XBR0      = 0x1F;
    XBR2      = 0x44;
		
		trig_ultrason = 0;
		trig_ultrason_AR = 0;
}

void Demo_GPIO(void)
{
}
