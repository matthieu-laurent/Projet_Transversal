//------------------------------------------------------------------------------------
#ifndef C8051F020
#define C8051F020
#include <C8051F020.h>
#endif
#include <LIB_BSE_UART.h>
#include <stdlib.h> // atoi
#include <stdio.h> // printf
#include <FO_M2__Structures_COMMANDES_INFORMATIONS_Serializer.h>

#define SYSCLK 22118400
#define BAUDRATE 19200

//*****************************************************************************************************************************************
// Fonction d'initialisation
//*****************************************************************************************************************************************

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

//*****************************************************************************************************************************************
// Fonction du Serializer
//*****************************************************************************************************************************************

void Init_Commande_Serializer()
{
		S_INPUTS.Etat_Commande=Commande_non;
    S_INPUTS.Ticks_mot1=0;               // paramètre Distance - Encoder Ticks moteur 1 (digo
    S_INPUTS.Vitesse_Mot1=0;             // paramètre de vitesse de rotation du moteur 1  (mogo digo
    S_INPUTS.Ticks_mot2=0;               // paramètre Distance - Encoder Ticks moteur 2 (digo
    S_INPUTS.Vitesse_Mot2=0;
    S_OUTPUTS.Etat_Response=Reponse_non;
}

void Avancer(int vitesse)				// vitesse=45 => avance à 45% de sa vitesse max
{
	S_INPUTS.Vitesse_Mot1=vitesse;
	S_INPUTS.Vitesse_Mot2=vitesse;
	S_INPUTS.Etat_Commande=mogo_1_2;
}

void Reculer(int vitesse)				// vitesse=45 => reculer à 45% de sa vitesse max
{
	S_INPUTS.Vitesse_Mot1=-vitesse;
	S_INPUTS.Vitesse_Mot2=-vitesse;
	S_INPUTS.Etat_Commande=mogo_1_2;
}

void Arret()		
{
	S_INPUTS.Vitesse_Mot1=0;
	S_INPUTS.Vitesse_Mot2=0;
	S_INPUTS.Etat_Commande=Stop;
}

void Rotation_Droite_90(int vitesse)
{
	S_INPUTS.Ticks_mot2=975;
	S_INPUTS.Vitesse_Mot2=vitesse;
	S_INPUTS.Etat_Commande=digo_2;
}

void Rotation_Gauche_90(int vitesse)
{
	S_INPUTS.Ticks_mot1=975;
	S_INPUTS.Vitesse_Mot1=vitesse;
	S_INPUTS.Etat_Commande=digo_1;
}

void Rotation_180(int vitesse)
{
	S_INPUTS.Ticks_mot1=1972;
	S_INPUTS.Vitesse_Mot1=vitesse;
	S_INPUTS.Etat_Commande=digo_1;	
}

void Rotation_angle(int angle,int vitesse)
{
	S_INPUTS.Ticks_mot1=angle*10.9389;
	S_INPUTS.Vitesse_Mot1=vitesse;
	S_INPUTS.Etat_Commande=digo_1;	
}

//*****************************************************************************************************************************************
// Fonction de l'UART1
//*****************************************************************************************************************************************
/*
void CFG_Clock_UART1(void)
{
	CKCON|=0x40; // timer4 clock select
  T4CON      = 0x34;//Timer 4 enabled
  TH4       = 0xB8;
	TL4= 0x00;
	 EIE2=0x44;
    RCAP4L    = 0xDC;
    RCAP4H    = 0xFF;
}

void CFG_UART1(void)
{
	P0MDOUT   = 0x21;
  XBR2      = 0x44;//UART1 transmet en P00 reçoit en P01
	PCON = 0x10;//: UART1 Baud Rate Doubler Enable.
	SCON1 =0x50; // Mode1 : 8-Bit UART, Variable Baud Rate,
	SCON1|=0x02;
}*/
// Stan
void CFG_UART1(void)
{
    PCON |=0x10; // UART1 baud rate devided by two disabled
    SCON1 =0x50; // Mode1 : 8-Bit UART, Variable Baud Rate,
             //RI1 will only be activated if stop bit is logic level 1,
            //UART1 reception enabled   
    EIE2|=0x40;
	EA=1;
}
void CFG_Clock_UART1(void)
{ // UART1 utilise Timer4
	T4CON      = 0x34;//Timer 4 enabled
    CKCON|=0x40;
  RCAP4L    = 0xDC;
  RCAP4H    = 0xFF;
    T4CON     = 0x34;
}

bit FLAG_ECRITURE=1;

char Putchar(char c, char csg_tempo)
{
	while(!FLAG_ECRITURE);
	SBUF1=c; // Envoi du caractère c dans le registre de données de l'UART
	FLAG_ECRITURE=0;
	return c;	
}

void ISR_UART1(void) interrupt 20
{
	SCON1&=0xFD;
  FLAG_ECRITURE=1;
}

char Send_String(char *char_ptr)
{
	char cpt=0;
	char erreur;
	while(*char_ptr!=0){
        	erreur=Putchar(*char_ptr, 10);
					if(erreur==0) return 0;
        	char_ptr++;
        	cpt++;
	}
	return cpt;
}

char Getchar(void)
{
	char c;
	if((SCON1&0x01)==0x00) return 0; // (RI1==0) Flag=0, pas de caractère reçu
       	c=SBUF1;
       	SCON1=SCON1&0xFE;//RI1=0;
       	return c;
}




void Action_UART1()
{
	char str[5];
	
	if(S_INPUTS.Etat_Commande!=Commande_non)
	{
		switch(S_INPUTS.Etat_Commande)
		{
			case mogo_1_2 : 
				Send_String("mogo 1:");
				sprintf(str,"%d",(int)S_INPUTS.Vitesse_Mot1);
				Send_String(str);
				Send_String(" 2:");
				sprintf(str,"%d\r",(int)S_INPUTS.Vitesse_Mot2);
				Send_String(str);
			break;
			
			case digo_1 :  
				Send_String("digo 1:");
				sprintf(str,"%d",(int)S_INPUTS.Ticks_mot1);
				Send_String(str);
				Send_String(":");
				sprintf(str,"%d\r",(int)S_INPUTS.Vitesse_Mot1);
				Send_String(str);
			break;
			
			case digo_2 :  
				Send_String("digo 2:");
				sprintf(str,"%d",(int)S_INPUTS.Ticks_mot2);
				Send_String(str);
				Send_String(":");
				sprintf(str,"%d\r",(int)S_INPUTS.Vitesse_Mot2);
				Send_String(str);
			break;
			
			case digo_1_2 :  
				Send_String("digo 1:");
				sprintf(str,"%d",(int)S_INPUTS.Ticks_mot1);
				Send_String(str);
				Send_String(":");
				sprintf(str,"%d",(int)S_INPUTS.Vitesse_Mot1);
				Send_String(str);
				Send_String(" 2:");
				sprintf(str,"%d",(int)S_INPUTS.Ticks_mot2);
				Send_String(str);
				Send_String(":");
				sprintf(str,"%d\r",(int)S_INPUTS.Vitesse_Mot2);
				Send_String(str);
			break;
			
			case Stop :
				Send_String("stop\r");
			break;	
		}
		S_INPUTS.Etat_Commande=Commande_non;
	}
}
















