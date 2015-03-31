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
	S_INPUTS.Ticks_mot2=970;
	S_INPUTS.Vitesse_Mot2=vitesse;
	S_INPUTS.Etat_Commande=digo_2;
}

void Rotation_Gauche_90(int vitesse)
{
	S_INPUTS.Ticks_mot1=970;
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

void CFG_Clock_UART1(void)
{
	CKCON|=0x10; // timer1 clock select
	TMOD|=0x20; // l'horloge du timer 1 est sysclock
	TMOD&=0x2F; // le timer 1 est configuré en timer 8 bits avec autoreload
  TCON      = 0x40;
  TH1       = 0xB8;
	TL1= 0x00;
	ET1=0; // désactive les interruptions du timer 1
	TR1=1;//Demarrer le Timer1	
	TF1=0;//flag du Timer1 remis à 0
	
// On démarre le timer 1 à FFh ainsi un coup de clock après avoir démarrer, 
// le timer se précharge avec la valeur de TH1
	
}

void CFG_UART1(void)
{
	P0MDOUT   = 0x21;
  XBR2      = 0x44;//UART1 transmet en P00 reçoit en P01
	RCLK0=0; // horloges de réception de l'UART0 = Timer 1
	TCLK0=0; // horloges de transmission de l'UART0 = Timer 1
	//PCON |=0x80; // SMOD0=1 : UART0 baud rate devided by two disabled
	//PCON &= 0xBF; // SSTAT0=0
	PCON = 0x10;
	SCON1 =0x50; // Mode1 : 8-Bit UART, Variable Baud Rate,
		     //RI0 will only be activated if stop bit is logic level 1,
		    //UART0 reception enabled
	SCON1|=0x02;//TI1=1;
	EIE2|=0x40;
	EA=1;
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
	if(S_INPUTS.Etat_Commande!=Commande_non)
	{
		switch(S_INPUTS.Etat_Commande)
		{
			case mogo_1_2 : 
				Send_String("mogo 1:");
				Putchar('0'+S_INPUTS.Vitesse_Mot1,10);
				Send_String(" 2:");
				Putchar('0'+S_INPUTS.Vitesse_Mot2,10);
				Putchar('\r',10);
			break;
			
			case digo_1 :  
				Send_String("digo 1:");
				Putchar('0'+S_INPUTS.Ticks_mot1,10);
				Send_String(":");
				Putchar('0'+S_INPUTS.Vitesse_Mot1,10);
				Putchar('\r',10);
			break;
			
			case digo_2 :  
				Send_String("digo 2:");
				Putchar('0'+S_INPUTS.Ticks_mot2,10);
				Send_String(":");
				Putchar('0'+S_INPUTS.Vitesse_Mot2,10);
				Putchar('\r',10);
			break;
			
			case digo_1_2 :  
				Send_String("digo 1:");
				Putchar('0'+S_INPUTS.Ticks_mot1,10);
				Send_String(":");
				Putchar(S_INPUTS.Vitesse_Mot1,10);
				Send_String(" 2:");
				Putchar('0'+S_INPUTS.Ticks_mot2,10);
				Send_String(":");
				Putchar('0'+S_INPUTS.Vitesse_Mot2,10);
				Putchar('\r',10);
			break;
			
			case Stop :
				Send_String("stop");
				Putchar('\r',10);
			break;	
		}
		S_INPUTS.Etat_Commande=Commande_non;
	}
}
















