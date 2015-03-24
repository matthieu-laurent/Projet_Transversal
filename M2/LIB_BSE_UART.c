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
#define BAUDRATE 9600

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
	TH1= -(SYSCLK/BAUDRATE/16);
	TL1= 0xff;
	ET1=0; // désactive les interruptions du timer 1
	TR1=1;//Demarrer le Timer1	
	TF1=0;//flag du Timer1 remis à 0
// On démarre le timer 1 à FFh ainsi un coup de clock après avoir démarrer, 
// le timer se précharge avec la valeur de TH1
}

void CFG_UART1(void)
{
	RCLK0=0; // horloges de réception de l'UART0 = Timer 1
	TCLK0=0; // horloges de transmission de l'UART0 = Timer 1
	PCON |=0x80; // SMOD0=1 : UART0 baud rate devided by two disabled
	PCON &= 0xBF; // SSTAT0=0
	SCON1 =0x72; // Mode1 : 8-Bit UART, Variable Baud Rate,
		     //RI0 will only be activated if stop bit is logic level 1,
		    //UART0 reception enabled
	SCON1=SCON1|0x02;//TI1=1;
}

char Putchar(char c, char csg_tempo)
{
	int i;
	while(csg_tempo!=0)
	{
		if((SCON1&0x02)==0x00){
			for(i=0; i<440;i++){}//temporisation de 20µs
	        csg_tempo--;
			if(csg_tempo==0) return 0;
			}	
		else{
	        SBUF1=c; // Envoi du caractère c dans le registre de données de l'UART
        	SCON1=SCON1|0x02; // Remise à zéro du drapeau
        	return c;
			}
	}	
	return 0;
	
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
				Putchar(S_INPUTS.Vitesse_Mot1,10);
				Send_String(" 2:");
				Putchar(S_INPUTS.Vitesse_Mot2,10);
				Send_String(" <CR>");
			break;
			
			case digo_1 :  
				Send_String("digo 1:");
				Putchar(S_INPUTS.Ticks_mot1,10);
				Send_String(":");
				Putchar(S_INPUTS.Vitesse_Mot1,10);
				Send_String(" <CR>");
			break;
			
			case digo_2 :  
				Send_String("digo 2:");
				Putchar(S_INPUTS.Ticks_mot2,10);
				Send_String(":");
				Putchar(S_INPUTS.Vitesse_Mot2,10);
				Send_String(" <CR>");
			break;
			
			case digo_1_2 :  
				Send_String("digo 1:");
				Putchar(S_INPUTS.Ticks_mot1,10);
				Send_String(":");
				Putchar(S_INPUTS.Vitesse_Mot1,10);
				Send_String(" 2:");
				Putchar(S_INPUTS.Ticks_mot2,10);
				Send_String(":");
				Putchar(S_INPUTS.Vitesse_Mot2,10);
				Send_String(" <CR>");
			break;
			
			case Stop :
				Send_String("stop <CR>");
			break;	
		}
		S_INPUTS.Etat_Commande=Commande_non;
	}
}
















