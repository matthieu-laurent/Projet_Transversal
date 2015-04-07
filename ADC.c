//------------------------------------------------------------------------------------
// Projet BSE 2014 - Fichier configuration et gestion de l'ADC0
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
#include "LIB_BSE_ADC.h"

void CFG_VREF()
{
	REF0CN = 0x03; 
}

void CFG_ADC0()
{
	AMX0SL = 0x03; // Pin AIN0.3 en unipolaire
	
	//ADC0CF = 0xF9; // Gain de 2
	
	ADC0CN = 0x80; // Activer ADC0
	
	AD0LJST = 1; // Pas de signe sur la sortie -> 8 bits à lire sur ADC0H
}

unsigned char ACQ_ADC()
{
	AD0INT = 0;
	
	AD0BUSY = 1; // Déclenche une conversion
	
	while(!AD0INT);
	
	AD0BUSY = 0;
	
	return ADC0H;
}
