//------------------------------------------------------------------------------------
#include "commandes_info_serializer.h"
#include "Controle_Moteur.h"
#include <stdlib.h>
#include <stdio.h>

extern OUT_M2 xdata S_OUTPUTS;  // S_ : serializer
extern IN_M2 xdata S_INPUTS; 

//*****************************************************************************************************************************************
// Fonctions du Serializer
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

void Avancer_m(int vitesse)				// vitesse=45 => avance à 45% de sa vitesse max
{
	S_INPUTS.Vitesse_Mot1=vitesse;
	S_INPUTS.Vitesse_Mot2=vitesse;
	S_INPUTS.Etat_Commande=mogo_1_2;
}

void Avancer_distance(int vitesse,int distance)
{
	S_INPUTS.Vitesse_Mot1=vitesse;
	S_INPUTS.Ticks_mot1=distance*310;
	S_INPUTS.Vitesse_Mot2=vitesse;
	S_INPUTS.Ticks_mot2=distance*310;
	S_INPUTS.Etat_Commande=digo_1_2;
}

void Reculer_m(int vitesse)				// vitesse=45 => reculer à 45% de sa vitesse max
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

void Rotation_180(int vitesse, char sens)
{
	if(sens == 'G')
	{
		S_INPUTS.Ticks_mot1=1972;
		S_INPUTS.Vitesse_Mot1=vitesse;
		S_INPUTS.Etat_Commande=digo_1;	
	}
	else
	{
		S_INPUTS.Ticks_mot2=1972;
		S_INPUTS.Vitesse_Mot2=vitesse;
		S_INPUTS.Etat_Commande=digo_2;	
	}
	
}

void Rotation_angle(int angle,int vitesse, char sens)
{
	if(sens == 'G')
	{
		S_INPUTS.Ticks_mot1=angle*10.9389;
		S_INPUTS.Vitesse_Mot1=vitesse;
		S_INPUTS.Etat_Commande=digo_1;	
	}
	else
	{
		S_INPUTS.Ticks_mot2=angle*10.9389;
		S_INPUTS.Vitesse_Mot2=vitesse;
		S_INPUTS.Etat_Commande=digo_2;	
	}
}

void Aller_en(int x,int y,int angle,int vitesse)
{
	long int i,j;
	Avancer_distance(vitesse,x);
	Action_UART1();
	
	// TEMPO
	for(i=0;i<(x*60);i++)
	{
		for(j=0;j<1000;j++)
			_nop_();
	}
	if(angle<0)
		Rotation_angle(angle,vitesse,'G');
	else
		Rotation_angle(angle,vitesse,'D');
	Action_UART1();
	
	// TEMPO
	for(i=0;i<1000;i++)
	{
		for(j=0;j<500;j++)
			_nop_(); 
	}
	
	Avancer_distance(vitesse,y);
}

//****************************************************************************************************************************************

void Action_UART1()
{
	char str[5]="";	// BUFFER permet récupérer la conversion decimal->string pour les paramètres Vitesse, etc.
	
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















