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
#include <string.h>

typedef int bool;
#define true 1
#define false 0

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

void Analyse_String(char str[])
{
	char* xdata p = str;
	int xdata vitesse = 0;
	int xdata angle = 0;
	int xdata coordX = 0;
	int xdata coordY = 0;
	bool xdata sortie = false;
	int xdata cmp;
	
	while(*p != '\r' && sortie == false)
	{
		switch (*p)
		{
			case 'D':
				p++;
				if(*p != '\r') p++;
				switch (*p)
				{
					case '1':
						CMD_C.Etat_Epreuve = epreuve1;
						sortie = true;
						break;
					case '2':
						CMD_C.Etat_Epreuve = epreuve2;
						sortie = true;
						break;
					case '3':
						CMD_C.Etat_Epreuve = epreuve3;
						sortie = true;
						break;
					case '4':
						CMD_C.Etat_Epreuve = epreuve4;
						sortie = true;
						break;
					case '5':
						CMD_C.Etat_Epreuve = epreuve5;
						sortie = true;
						break;
					case '6':
						CMD_C.Etat_Epreuve = epreuve6;
						sortie = true;
						break;
					case '8':
						CMD_C.Etat_Epreuve = epreuve8;
						sortie = true;
						break;
					default:
						CMD_C.Etat_Epreuve = epreuve1;
						sortie = true;
						break;
				} // fin switch
				break; // fin case D
			case 'E' :
				CMD_C.Etat_Epreuve = Fin_Epreuve;
				sortie = true;
				break;
			case 'Q' :
				CMD_C.Etat_Epreuve = Stop_Urgence;
				sortie = true;
				break;
			case 'T' :
				p++;
				if(*p == 'V')
				{
					p++;
					p++;
					vitesse = (unsigned char) calculVitesse(p);
					if(vitesse != 0)
					{
						CMD_C.Vitesse = vitesse;
						sortie = true;
					}
					else
					{
						CMD_C.Vitesse = 0;
						sortie = true;
					}
				}
				else
				{
					sortie = true;
				}
				break; // fin case T
			case 'A':
				if(*p != '\r') p++;
				if(*p == '\r') // Pas de paramètre
				{
					sortie = true;
					CMD_C.Etat_Mouvement = Avancer;
					CMD_C.Vitesse = 20;
				}
				else if(*p != 'S')
				{
					p++;
					vitesse = calculVitesse(p);
					if(vitesse != 0)
					{
						CMD_C.Vitesse = vitesse;
						CMD_C.Etat_Mouvement = Avancer;
						sortie = true;
					}
					else
					{
						CMD_C.Etat_Mouvement = Mouvement_non;
						CMD_C.Vitesse = 0;
						sortie = true;
					}
				}
				else if(*p == 'S')
        {  
					int xdata a = 0;
					p++;
          if(*p == 'S')
          {
						p++;
            if(*p == '\r') // Pas de paramètre
            {                 
							CMD_C.ACQ_Duree = 0;
              CMD_C.Etat_ACQ_Son = ACQ_non;
              sortie = true;
            }
            if(*p != '\r')
            {
							char xdata tab[4];
							p++;
              while(*p >= '0' && *p <= '9' && *p != '\0' && a<3)
              {
                tab[a] = *p;
                a++;
                p++;
              }
              tab[a]='\0';                               
              if(atoi(tab) >= 1 && atoi(tab) <= 99)
              {
                CMD_C.ACQ_Duree = atoi(tab);
                CMD_C.Etat_ACQ_Son =ACQ_oui;
                sortie = true;
              }
							else
							{
								CMD_C.Etat_ACQ_Son =ACQ_non;
                sortie = true;
							}
            }
          }
        }
				break; // fin case A
				
			case 'B':
				if(*p != '\r') p++;
				if(*p == '\r') // Pas de paramètre
				{
					CMD_C.Vitesse = 20;
					CMD_C.Etat_Mouvement = Reculer;
					sortie = true;
				}
				else
				{
					p++;
					vitesse = calculVitesse(p);
					if(vitesse != 0)
					{
						CMD_C.Vitesse = vitesse;
						CMD_C.Etat_Mouvement = Reculer;
						sortie = true;
					}
					else
					{
						CMD_C.Etat_Mouvement = Mouvement_non;
						CMD_C.Vitesse = 0;
						sortie = true;
					}
				}
				break; // fin case B
			
			case 'S':
				if(*p != '\r') p++;
				if(*p == '\r')
				{
					CMD_C.Etat_Mouvement = Stopper;
					sortie = true;
				}
				break;
			case 'R':
				p++;
				if(*p == 'D') // Rotation à droite de 90°
				{
					p++;
					if(*p == '\r')
					{
						CMD_C.Etat_Mouvement = Rot_90D;
						sortie = true;
					}
				}
				else if(*p == 'G') // Rotation à gauche de 90°
				{
					p++;
					if(*p == '\r')
					{
						CMD_C.Etat_Mouvement = Rot_90G;
						sortie = true;
					}
				}
				else if (*p == 'C') // Rotation complète de la base de 180°
				{
					if(*p != '\r') p++;
					if(*p == '\r')
					{
						CMD_C.Etat_Mouvement = Rot_180D;
						sortie = true;
					}
					else
					{
						p++;
						if(*p == 'G')
						{
							CMD_C.Etat_Mouvement = Rot_180G;
							sortie = true;
						}
						else
						{
							CMD_C.Etat_Mouvement = Rot_180D;
							sortie = true;
						}
					}
				}
				else if (*p == 'A')
				{
					if(*p != '\r') p++;
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
								if(angle > 0 && angle <= 180)
								{
									CMD_C.Angle = angle;
									CMD_C.Etat_Mouvement = Rot_AngD;
									sortie = true;
								}
								else
								{
									CMD_C.Etat_Mouvement = Rot_90D;
									sortie = true;
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
								if(angle > 0 && angle <= 180)
								{
									CMD_C.Angle = angle;
									CMD_C.Etat_Mouvement = RotAngG;
									sortie = true;
								}
								else
								{
									CMD_C.Etat_Mouvement = Rot_90D;
									sortie = true;
								}
							}
							else
							{
								CMD_C.Etat_Mouvement = Rot_90D;
								sortie = true;
							}
						}
						else
						{
							CMD_C.Etat_Mouvement = Mouvement_non;
							sortie = true;
						}
						
					}
					else if (*p == '\r') // Pas de paramètres donc cas par défaut
					{
						CMD_C.Etat_Mouvement = Rot_90D;
						sortie = true;
					}
				}
				break; // fin case R
				
			case 'G':
				p++;
				if(*p == ' ')
				{
					p++;
					if(*p == 'X')
					{
						p++;
						if(*p == ':')
						{
							p++;
							p = calculCoord(p,'X');
							sortie = true;
						}
						p++;
					}
					else
					{
						CMD_C.Etat_Mouvement = Mouvement_non;
						sortie = true;
					}
					if(*p == 'Y')
					{
						p++;
						if(*p == ':')
						{
							p++;
							p = calculCoord(p,'Y');
							sortie = true;
						}
						p++;
					}
					else
					{
						CMD_C.Etat_Mouvement = Mouvement_non;
						sortie = true;
					}
					if(*p == 'A')
					{
						p++;
						if(*p == ':')
						{
							p++;
							CMD_C.Angle = calculAngle(p);
							CMD_C.Etat_Mouvement = Mouvement_non;
							sortie = true;
						}
					}
					else
					{
						CMD_C.Etat_Mouvement = Mouvement_non;
						sortie = true;
					}
				}
				else
					{
						CMD_C.Etat_Mouvement = Mouvement_non;
						sortie = true;
					}
				break; // fin case G
					
			case 'M':    
				
				p++;
        if(*p == 'O')
        {
          p++;
          if(*p == 'B')
          //TRAITEMENT DU PARAMETRE D    
          { 
						p++;
            if(*p == '\r')//pas de parametres
            {
              CMD_C.Etat_DCT_Obst = oui_360;
              cmp=1;
            }
            else//presence espace ou caractere
            {
							int xdata a = 0;
							char xdata tab[5];
              if(*p == ' ')//si espace
              {
								p++;
							}
              while(*p >= '0' && *p <= '9' && *p != '\0' && a<3)    
              {
								tab[a] = *p;
                a++;
                p++;
              }
              tab[a]='\0';
              if(atoi(tab) == 180 )
              {
                CMD_C.Etat_DCT_Obst = oui_180;
                cmp=1;
              }
              else
              {
								CMD_C.Etat_DCT_Obst = oui_360;
                cmp=1;
              }
              p++;
              if(*p == ' ')//si espace
              {
								p++;
							}
              //TRAITEMENT DU PARAMETRE A:
              if(*p =='A')
              {    
								p++;
                if(*p == '\r')//pas de parametres
                {
										CMD_C.DCT_Obst_Resolution =30;
										cmp++;
								}	
                if(*p ==':')
                {  
									p++;
                  a=0;
                  while(*p >= '0' && *p <= '9' && *p != '\0' && a<2)    
                  {
										tab[a] = *p;
                    a++;
                    p++;
                  }
                  tab[a]='\0';
                  if(atoi(tab) >= 5 && atoi(tab) <= 45 )
                  {
										CMD_C.DCT_Obst_Resolution = atoi(tab);
                    cmp++;
                  }
                  else
                  {
                    CMD_C.DCT_Obst_Resolution =30;
										cmp++;
                  }
                  if(cmp == 2)
                  {
										sortie = true;
                  }
                }        
							}  
						}
					}
				}
				break;
				
				case 'L':
					p++;
					if(*p == ' ')
					{
						p++;
						if(*p == 'I')
						{
							CMD_C.Etat_Lumiere = Allumer;
							p++; // :
							p++; // Intensité
							p = calculIntensite(p); // D
							p++; // :
							p++; // Durée allumage
							p = calculDureeAllumage(p); // E 
							p++; // :
							p++; // Durée extinction
							p = calculDureeExtinction(p); // N
							p++; // : 
							p++; // Nombre
							calculNombre(p);
							sortie = true;
						}
					}
					else if(*p == '\r')
					{
						CMD_C.Etat_Lumiere = Allumer;
						CMD_C.Lumiere_Intensite = 100;
						CMD_C.Lumiere_Duree = 99;
						CMD_C.Lumiere_Extinction = 0;
						CMD_C.Lumiere_Nbre = 1;
						sortie = true;
					}
					else if(*p == 'S')
					{
						p++;
						if(*p == '\r')
						{
							CMD_C.Etat_Lumiere = Eteindre;
							sortie = true;
						}
					}
					else
					{
						sortie = true;
					}
					break;
				case 'C':
					p++;
					if(*p == 'S')
					{
						p++; // Espace
						p++; // Lettre
						switch(*p)
						{
							case 'C':
								CMD_C.Etat_Servo = Servo_C;
								break;
							case 'H':
								CMD_C.Etat_Servo = Servo_H;
								break;
							case 'V':
								CMD_C.Etat_Servo = Servo_V;
								break;
							default:
								CMD_C.Etat_Servo = Servo_non;
								break;
						}
						if(*p != '\r')
						{
							p++; // Espace
							p++;
							if(*p == 'A')
							{
								p++; // :
								p++; // Valeur angle
								CMD_C.Servo_Angle = calculAngleServo(p);
							}
						}								
					}
					sortie = true;
					break;
					default:
						sortie = true;
						break;
			} // fin switch
	} // while
} // fin fonction

int calculVitesse(char* p)
{
	
	char xdata vitesse[5];
	
	int xdata i = 0;
	
	while(*p >= '0' && *p <= '9' && *p != '\r')
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

int calculAngle (char* p)
{
	
	char xdata angle[6];
	
	int xdata i = 0;
	
	while((*p >= '0' && *p <= '9') || *p=='-')
	{
		angle[i] = *p;
		i++;
		p++;
	}
	angle[i]='\0';
					
	if(atoi(angle) >= 0 && atoi(angle) <= 90)
		return atoi(angle);
	else if(atoi(angle) >= -90 && atoi(angle) < 0)
		return atoi(angle) + 256;
	else
		return 0;

}
int calculAngleServo (char* p)
{	
	char xdata angle[6];
	
	int xdata i = 0;
	
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

char* calculCoord(char* p, char c)
{	
	char xdata coord[4];
	
	int xdata i = 0;
	int xdata t = 0;
	
	while((*p >='0' && *p <='9') || *p == '-')
	{
		coord[i]= *p;
		i++;
		p++;
	}
	coord[i] = '\0';
	
  t	= atoi(coord);
	t = 256 + t;
	
	if(c == 'X')
	{
		if(atoi(coord) >= 0 && atoi(coord) <= 99)
			CMD_C.Coord_X = atoi(coord);
		else if(atoi(coord) >= -99 && atoi(coord) <0)
			CMD_C.Coord_X = atoi(coord) + 256;
		else
			CMD_C.Coord_X = 0;
	}
	else if(c == 'Y')
	{
		if(atoi(coord) >= - 99 && atoi(coord) <= 99)
			CMD_C.Coord_Y = atoi(coord);
		else if(atoi(coord) >= -99 && atoi(coord) <0)
			CMD_C.Coord_Y = atoi(coord) + 256;
		else
			CMD_C.Coord_Y = 0;
	}
	
	
	return p;
	
}

char* calculIntensite(char* p) // Traite et renvoie le pointeur sur la lettre suivante
{
	char xdata intensite[4];
	int xdata i = 0;
	while(*p >= '0' && *p <= '9' && *p != '\r')
	{
		intensite[i] = *p;
		p++;
		i++;
	}
	if(*p != '\r') p++;
	intensite[i] = '\0';
	if(atoi(intensite) >= 1 && atoi(intensite) <=100)
	{
		CMD_C.Lumiere_Intensite = atoi(intensite);
	}
	else
	{
		CMD_C.Lumiere_Intensite = 100;
	}
	return p;
}

char* calculDureeAllumage(char* p) // Traite et renvoie le pointeur sur la lettre suivante
{
	char xdata duree[4];
	int xdata i = 0;
	while(*p >= '0' && *p <= '9' && *p != '\r')
	{
		duree[i] = *p;
		p++;
		i++;
	}
	if(*p != '\r') p++;
	duree[i] = '\0';
	if(atoi(duree) >= 1 && atoi(duree) <=99)
	{
		CMD_C.Lumiere_Duree = atoi(duree);
	}
	else
	{
		CMD_C.Lumiere_Duree = 99;
	}
	return p;
}

char* calculDureeExtinction(char* p) // Traite et renvoie le pointeur sur la lettre suivante
{
	char xdata duree[4];
	int xdata i = 0;
	while(*p >= '0' && *p <= '9' && *p != '\r')
	{
		duree[i] = *p;
		p++;
		i++;
	}
	if(*p != '\r') p++;
	duree[i] = '\0';
	if(atoi(duree) >= 0 && atoi(duree) <=99)
	{
		CMD_C.Lumiere_Extinction = atoi(duree);
	}
	else
	{
		CMD_C.Lumiere_Extinction = 0;
	}
	return p;
}

void calculNombre(char* p)
{
	char xdata nombre[4];
	int xdata i = 0;
	while(*p >= '0' && *p <= '9' && *p != '\r')
	{
		nombre[i] = *p;
		p++;
		i++;
	}
	if(*p != '\r') p++;
	nombre[i] = '\0';
	if(atoi(nombre) >= 1 && atoi(nombre) <=99)
	{
		CMD_C.Lumiere_Nbre = atoi(nombre);
	}
	else
	{
		CMD_C.Lumiere_Nbre = 1;
	}
}

char* Invite_Commandes() //message signalant que le robot est pret
{
      return "I le robot est pret\r";
}

char* Arrivee_point()//message signalant que le robot est arrivée au pointspécifie
{
      return "Arrivee au point specifie\r";
}

char* Servomoteur_positionne(char* b)//message signalant que le servomoteur a atteint sa position de consigne
{
   return strcat("AS ",b);
}

char* Detection_obstacle(int angle,int distance)
{
  char xdata chaine[15];
	sprintf(chaine,"KOB %d: %d\r",angle,distance);
  return chaine;
}

char* Info_auxilliaire(char* a)//information Info_auxilliaire
{
    return strcat("IA ",a);
}

char* Accuse_Reception_OK()
{
	return "\r\n>";
}

char* Accuse_Reception_KO()
{
	return "\r\n#";
}
