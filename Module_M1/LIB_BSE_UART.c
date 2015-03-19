//------------------------------------------------------------------------------------
// Projet BSE 2014 - Fichier configuration et gestion de l'UART0
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
#include "LIB_BSE_UART.h"
#include "FO_M1__Structures_COMMANDES_INFORMATIONS_CentraleDeCommande.h"
#include <stdlib.h>     // atoi 
#include <stdio.h>   // printf

void CFG_Clock_UART0()
{
	CKCON |= 0x10; // T1M : Timer 1 Clock Select
	
	TMOD |= 0x20; // On met T1M1 à 1
	
	TMOD &= 0x2F; // On met T1M0 à 0
	
	// Timer 1 configuré en timer 8 bits avec auto-reload
	
	TF1 = 0; // On met à 0 le bit d'overflow du Timer 1
	
	TH1 = -(SYSCLK / BAUDRATE / 16);
	
	ET1 = 1; // Interruption Timer 1 désactivée
	
	TR1 = 1; // Timer 1 démarré


}

void CFG_UART0()
{
	RCLK0 = 0; // Bit 5 de T2CON : source clk Timer 1
	
	TCLK0 = 0; // Bit 4 de T2CON : transmit clk flag Timer 1
	
	PCON |= 0x80; // On met à 1 SMOD0 pour désactiver le doubleur de baudrate
	
	PCON &= 0x0BF; // On met à 0 SSTAT0
	
	SCON0 = 0x72; // Mode 1 - Check Stop Bit - Réception validée - Transmission - Octet transmis (prêt à recevoir un char pour transmettre)
	//SCON0 = 0x50;
}

char Putchar(char c, char csg_tempo)
{
	int tempo = (int)csg_tempo;
	int i = 0;
	
	while (!TI0)
	{
		for(i=0 ; i<443 ; i++); // Temporisation 20us
		
		tempo--;
		
		if (tempo == 0) return 0;
		
	}
	
	SBUF0 = c;
	
	TI0=0;
	
	return c;
}

char Send_String(char *char_ptr)
{
	int i = 0;
	char ret = 0;
	int erreur = 0;
	char cpt = 0;
	
	while(char_ptr[i] != '\0')
	{
		ret = Putchar(char_ptr[i],10);
		
		i++;
		
		if(ret == 0) erreur = 1;
		if(ret != 0) cpt++;
	}
	
	switch(erreur)
	{
		case 0 : return cpt;
		
		case 1 : return 0;
		
		default : return 0;
	}
}

char Getchar()
{
	char car = 0;
	
	if(RI0 == 0) return car;
		
	car = SBUF0;
	
	RI0 = 0;
	
	return car;
}

void Demo_UART()
{
	int i;
	char c= 'a';
	char tempo = 'a';
	char str[20] = "Bonjour";
	
	Putchar(c,tempo);
	
	for(i=0;i<1000;i++);
	
	c = 'b';
	
	Putchar(c,tempo);
	
	for(i=0;i<1000;i++);
	
	c = 'c';
	
	Putchar(c,tempo);
	
	for(i=0;i<1000;i++);
	
	Send_String(str);
	
	for(i=0;i<1000;i++);
	TI0 = 0;
	
	c = Getchar();
	
}

void Analyse_String(char* str)
{
	char *p = str;
	int vitesse = 0;
	int angle = 0;
	int coordX = 0;
	int coordY = 0;
	
	while(*p != '\r')
	{
		switch (*p)
		{
			case 'D':
				p++;
				p++;
				switch (*p)
				{
					case '1':
						out.Etat_Epreuve = epreuve1;
						p++;
						break;
					case '2':
						out.Etat_Epreuve = epreuve2;
						p++;
						break;
					case '3':
						out.Etat_Epreuve = epreuve3;
						p++;
						break;
					case '4':
						out.Etat_Epreuve = epreuve4;
						p++;
						break;
					case '5':
						out.Etat_Epreuve = epreuve5;
						p++;
						break;
					case '6':
						out.Etat_Epreuve = epreuve6;
						p++;
						break;
					case '8':
						out.Etat_Epreuve = epreuve8;
						p++;
						break;
					default:
						out.Etat_Epreuve = epreuve1;
						p++;
						break;
				}
				break;
			case 'E' :
				out.Etat_Epreuve = Fin_Epreuve;
				break;
			case 'Q' :
				out.Etat_Epreuve = Stop_Urgence;
				break;
			case 'T' :
				p++;
				if(*p == 'V')
				{
					p++;
					vitesse = calculVitesse(p);
					if(vitesse != 0)
					{
						out.Vitesse = vitesse;
					}
					else
					{
						out.Vitesse = 0;
						break;
					}
				}
				else
				{
					break;
				}
			case 'A':
				p++;
				if(*p == '\r') // Pas de paramètre
				{
					out.Vitesse = 20;
					out.Etat_Mouvement = Avancer;
				}
				
				if(*p != 'S')
				{
					
					vitesse = calculVitesse(p);
					if(vitesse != 0)
					{
						out.Vitesse = vitesse;
						out.Etat_Mouvement = Avancer;
					}
					else
					{
						out.Etat_Mouvement = Mouvement_non;
					}
					break;
				}
				if(*p == 'S')
					// ASS
				break;
				
			case 'B':
				p++;
				if(*p == '\r') // Pas de paramètre
				{
					out.Vitesse = 20;
					out.Etat_Mouvement = Reculer;
				}
				
				vitesse = calculVitesse(p);
				if(vitesse != 0)
				{
					out.Vitesse = vitesse;
					out.Etat_Mouvement = Reculer;
				}
				else
				{
					out.Etat_Mouvement = Mouvement_non;
				}
				break;
			
			case 'S':
				p++;
				if(*p == '\r')
				{
					out.Etat_Mouvement = Stopper;
				}
			case 'R':
				p++;
				if(*p == 'D') // Rotation à droite de 90°
				{
					p++;
					if(*p == '\r')
						out.Etat_Mouvement = Rot_90D;
				}
				else if(*p == 'G') // Rotation à gauche de 90°
				{
					p++;
					if(*p == '\r')
						out.Etat_Mouvement = Rot_90G;
				}
				else if (*p == 'C') // Rotation complète de la base de 180°
				{
					p++;
					if(*p == '\r')
						out.Etat_Mouvement = Rot_180D;
					else
					{
						p++;
						if(*p == 'D')
							out.Etat_Mouvement = Rot_180D;
						else if(*p == 'G')
							out.Etat_Mouvement = Rot_180G;
					}
				}
				else if (*p == 'A')
				{
					p++;
					if(*p == ' ')
					{
						p++;
						if(*p == 'D')
						{
							p++;
							if(*p == ':')
							{
								p++;
								angle = calculAngle(p);
								if(angle >= 0 && angle <= 180)
								{
									out.Angle = angle;
									out.Etat_Mouvement = Rot_AngD;
								}
							}
						}
						else if(*p == 'G')
						{
							p++;
							if(*p == ':')
							{
								p++;
								angle = calculAngle(p);
								if(angle >= 0 && angle <= 180)
								{
									out.Angle = angle;
									out.Etat_Mouvement = RotAngG;
								}
							}
						}
						else
						{
							out.Etat_Mouvement = Mouvement_non;
						}
						
					}
					else if (*p == '\r') // Pas de paramètres donc cas par défaut
					{
						out.Etat_Mouvement = Rot_90D;
					}
					break;
				}
				break;
				
			case 'G':
				p++;
				if(*p == ' ')
				{
					if(*p == 'X')
					{
						p++;
						if(*p == ':')
						{
							p++;
							p = calculCoord(p,'X');
						}
						p++;
					}
					if(*p == 'Y')
					{
						p++;
						if(*p == ':')
						{
							p++;
							p = calculCoord(p,'Y');
						}
						p++;
					}
					if(*p == 'A')
					{
						p++;
						if(*p == ':')
						{
							p++;
							out.Angle = calculAngle(p);
						}
					}
				} // fin case G
				break;
			} // fin switch
	} // while
} // fin fonction

int calculVitesse(char *str)
{
	char *p = str;
	
	char vitesse[5];
	
	int i = 0;
	
	while(*p >= '0' && *p <= '9' && *p != '\0')
	{
		vitesse[i] = *p;
		i++;
		p++;
	}
	vitesse[i]='\0';
					
	if(vitesse >= 10 && atoi(vitesse) <= 100)
		return atoi(vitesse);
	else
		return 0;
}

int calculAngle (char* str)
{
	char *p = str;
	
	char angle[6];
	
	int i = 0;
	
	while((*p >= '0' && *p <= '9') || *p=='-')
	{
		angle[i] = *p;
		i++;
		p++;
	}
	angle[i]='\0';
					
	if(atoi(angle) >= -180 && atoi(angle) <= 180)
		return atoi(angle);
	else
		return 0;

}

char* calculCoord(char* str, char c)
{
	char *p =str;
	
	char coord[4];
	
	int i = 0;
	
	while((*p >='0' && *p <='9') || *p == '-')
	{
		coord[i]= *p;
		i++;
		p++;
	}
	coord[i] = '\0';
	
	if(c == 'X')
	{
		if(atoi(coord) >= - 99 && atoi(coord) <= 99)
			out.Coord_X = atoi(coord);
		else
			out.Coord_X = 0;
	}
	else if(c == 'Y')
	{
		if(atoi(coord) >= - 99 && atoi(coord) <= 99)
			out.Coord_Y = atoi(coord);
		else
			out.Coord_Y = 0;
	}
	
	
	return p;
	
}
