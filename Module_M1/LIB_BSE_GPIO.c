//------------------------------------------------------------------------------------
// Projet BSE 2014 - Fonctions de configuration et d'utilisation des GPIOs
//------------------------------------------------------------------------------------
//
// AUTH: FARAMOND Victor & CHENE Charly
// DATE: 06-10-2014
// Target: C8051F02x

//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include "c8051f020.h"                   // SFR declarations
#include "Ports_Declarations_TP_Emb.h"
#include "LIB_BSE_GPIO.h"
#include <intrins.h>

void Config_GPIO()
{
		// P0.0  -  TX0 (UART0), Push-Pull,  Digital
    // P0.1  -  RX0 (UART0), Open-Drain, Digital
    // P0.2  -  CEX0 (PCA),  Push-Pull,  Digital
    // P0.3  -  CEX1 (PCA),  Open-Drain, Digital
    // P0.4  -  CEX2 (PCA),  Open-Drain, Digital
    // P0.5  -  CEX3 (PCA),  Open-Drain, Digital
    // P0.6  -  ECI,         Open-Drain, Digital
    // P0.7  -  T0 (Timer0), Push-Pull,  Digital

    // P1.0  -  INT0 (Tmr0),         Open-Drain, Digital
    // P1.1  -  T1 (Timer1),         Open-Drain, Digital
    // P1.2  -  INT1 (Tmr1),         Open-Drain, Digital
    // P1.3  -  T2 (Timer2),         Open-Drain, Digital
    // P1.4  -  T2EX (Tmr2),         Open-Drain, Digital
    // P1.5  -  SYSCLK,              Push-Pull,  Digital
    // P1.6  -  General Purpose I/O, Push-Pull,  Digital   Port LED
    // P1.7  -  General Purpose I/O, Open-Drain, Digital

    // P2.0  -  General Purpose I/O, Open-Drain, Digital
    // P2.1  -  General Purpose I/O, Open-Drain, Digital
    // P2.2  -  General Purpose I/O, Open-Drain, Digital
    // P2.3  -  General Purpose I/O, Open-Drain, Digital
    // P2.4  -  General Purpose I/O, Open-Drain, Digital
    // P2.5  -  General Purpose I/O, Open-Drain, Digital
    // P2.6  -  General Purpose I/O, Open-Drain, Digital
    // P2.7  -  General Purpose I/O, Open-Drain, Digital

    // P3.0  -  General Purpose I/O, Open-Drain, Digital
    // P3.1  -  General Purpose I/O, Open-Drain, Digital
    // P3.2  -  General Purpose I/O, Open-Drain, Digital
    // P3.3  -  General Purpose I/O, Open-Drain, Digital
    // P3.4  -  General Purpose I/O, Open-Drain, Digital
    // P3.5  -  General Purpose I/O, Open-Drain, Digital
    // P3.6  -  General Purpose I/O, Open-Drain, Digital
    // P3.7  -  General Purpose I/O, Open-Drain, Digital

    P0MDOUT   = 0x85;
    P1MDOUT   = 0x60;
    
    P3MDOUT   = 0x00;
    P74OUT    = 0x00;
    P1MDIN    = 0xFF;

    XBR0      = 0x64;
    XBR1      = 0xFE;
    XBR2      = 0x40;

    P2        = 0x00;
    P1        = 0x00;
}

void Pulse_P20() // Génération d'une impulsion de 500ns sur P2.0 (Poussoir A)
{
	int i = 0;
	
	Decl_PP1 = 1;
	
	//for(i=0;i<1;i++); // Attente de 500ns
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	
	Decl_PP1 = 0;
}

void Pulse_P21() // Génération d'une impulsion de 500ns sur P2.1 (Poussoir B)
{
	int i = 0;
	
	Decl_PP2 = 1;
	
	//for(i=0;i<1;i++); // Attente de 500ns
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	
	Decl_PP2 = 0;
}

void Demo_GPIO()
{
	int i = 0;
	
	Decl_PP1 = 0;
	Decl_PP2 = 0;
	
	for(i=0;i<100;i++);
	
	Pulse_P20();
	
	Pulse_P21();
		
	CHG_DCT = 1;
	Tst4 = 1;
	Tst5 = 1;
	Tst6 = 1;
	Tst7 = 1;
	Decl_PES = 1;
	
	while(1)
	{
				
		if(DETECT == 1) SIG_Erreur = 1;
		
		if(DETECT == 0) SIG_Erreur = 0;
		
		if(RAZ_RTC == 1) CT1_DCT = 1;
		
		if(RAZ_RTC == 0) CT1_DCT = 0;
		
		if(START_Sys == 1) CT2_DCT = 1;
		
		if(START_Sys == 0) CT2_DCT = 0;
		
		if(RAZ_CP == 1) CT3_DCT = 1;
		
		if(RAZ_CP == 0) CT3_DCT = 0;
	}
	
	
}
