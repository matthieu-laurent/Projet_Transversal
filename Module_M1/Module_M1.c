//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include <stdlib.h>     // atoi 
#include <stdio.h>   // printf
#include "commandes_info_centrale.h"
#include "Module_M1.h"
#include <string.h>

extern OUT_M1 xdata cmd_c; // cmd_c : commande centrale
char xdata *msg_info;
// Renvoie 1 si message ok, 0 sinon

int Analyse_String(char str[])
{
	char* xdata p = str;
	int xdata vitesse = 0;
	int xdata angle = 0;
	int xdata coordX = 0;
	int xdata coordY = 0;
	bool xdata sortie = false;
	
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
						cmd_c.Etat_Epreuve = epreuve1;
						return 1;
						break;
					case '2':
						cmd_c.Etat_Epreuve = epreuve2;
						return 1;
						break;
					case '3':
						cmd_c.Etat_Epreuve = epreuve3;
						return 1;
						break;
					case '4':
						cmd_c.Etat_Epreuve = epreuve4;
						return 1;
						break;
					case '5':
						cmd_c.Etat_Epreuve = epreuve5;
						return 1;
						break;
					case '6':
						cmd_c.Etat_Epreuve = epreuve6;
						return 1;
						break;
					case '8':
						cmd_c.Etat_Epreuve = epreuve8;
						return 1;
						break;
					default:
						cmd_c.Etat_Epreuve = epreuve1;
						return 1;
						break;
				} // fin switch
				break; // fin case D
			case 'E' :
				cmd_c.Etat_Epreuve = Fin_Epreuve;
				return 1;
				break;
			case 'Q' :
				cmd_c.Etat_Epreuve = Stop_Urgence;
				return 1;
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
						cmd_c.Vitesse = vitesse;
						return 1;
					}
					else
					{
						cmd_c.Vitesse = 0;
						return 0;
					}
				}
				else
				{
					return 0;
				}
				break; // fin case T
			case 'A':
				if(*p != '\r') p++;
				if(*p == '\r') // Pas de paramètre
				{
					if(cmd_c.Vitesse <10)
					{
						cmd_c.Etat_Mouvement = Avancer;
						cmd_c.Vitesse = 20;
						return 1;
					}
					cmd_c.Etat_Mouvement = Avancer;
					return 1;
				}
				else if(*p != 'S')
				{
					p++;
					vitesse = calculVitesse(p);
					if(vitesse != 0)
					{
						cmd_c.Vitesse = vitesse;
						cmd_c.Etat_Mouvement = Avancer;
						return 1;
					}
					else
					{
						cmd_c.Etat_Mouvement = Mouvement_non;
						cmd_c.Vitesse = 0;
						return 0;
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
							cmd_c.ACQ_Duree = 0;
              cmd_c.Etat_ACQ_Son = ACQ_non;
              return 1;
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
                cmd_c.ACQ_Duree = atoi(tab);
                cmd_c.Etat_ACQ_Son =ACQ_oui;
								return 1;
              }
							else
							{
								cmd_c.Etat_ACQ_Son =ACQ_non;
								return 0;
							}
            }
          }
        }
				break; // fin case A
				
			case 'B':
				if(*p != '\r') p++;
				if(*p == '\r') // Pas de paramètre
				{
					if(cmd_c.Vitesse <10)
					{
						cmd_c.Etat_Mouvement = Reculer;
						cmd_c.Vitesse = 20;
						return 1;
					}
					cmd_c.Etat_Mouvement = Reculer;
					return 1;
				}
				else
				{
					p++;
					vitesse = calculVitesse(p);
					if(vitesse != 0)
					{
						cmd_c.Vitesse = vitesse;
						cmd_c.Etat_Mouvement = Reculer;
						return 1;
					}
					else
					{
						cmd_c.Etat_Mouvement = Mouvement_non;
						cmd_c.Vitesse = 0;
						return 0;
					}
				}
				break; // fin case B
			
			case 'S':
				if(*p != '\r') p++;
				if(*p == '\r')
				{
					cmd_c.Etat_Mouvement = Stopper;
					return 1;
				}
				break;
			case 'R':
				p++;
				if(*p == 'D') // Rotation à droite de 90°
				{
					p++;
					if(*p == '\r')
					{
						cmd_c.Etat_Mouvement = Rot_90D;
						return 1;
					}
				}
				else if(*p == 'G') // Rotation à gauche de 90°
				{
					p++;
					if(*p == '\r')
					{
						cmd_c.Etat_Mouvement = Rot_90G;
						return 1;
					}
				}
				else if (*p == 'C') // Rotation complète de la base de 180°
				{
					if(*p != '\r') p++;
					if(*p == '\r')
					{
						cmd_c.Etat_Mouvement = Rot_180D;
						return 1;
					}
					else
					{
						p++;
						if(*p == 'G')
						{
							cmd_c.Etat_Mouvement = Rot_180G;
							return 1;
						}
						else if(*p == 'D')
							{
								cmd_c.Etat_Mouvement = Rot_180D;
								return 1;
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
									cmd_c.Angle = angle;
									cmd_c.Etat_Mouvement = Rot_AngD;
									return 1;
								}
								else
								{
									cmd_c.Etat_Mouvement = Rot_90D;
									return 1;
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
									cmd_c.Angle = angle;
									cmd_c.Etat_Mouvement = Rot_AngG;
									return 1;
								}
								else
								{
									cmd_c.Etat_Mouvement = Rot_90D;
									return 1;
								}
							}
							else
							{
								cmd_c.Etat_Mouvement = Rot_90D;
								return 1;
							}
						}
						else
						{
							cmd_c.Etat_Mouvement = Mouvement_non;
							return 1;
						}
						
					}
					else if (*p == '\r') // Pas de paramètres donc cas par défaut
					{
						cmd_c.Etat_Mouvement = Rot_90D;
						return 1;
					}
				}
				break; // fin case R
				
			case 'G':			// A REFAIRE
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
						}
						p++;
					}
					else
					{
						cmd_c.Etat_Mouvement = Mouvement_non;
						return 0;
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
					else
					{
						cmd_c.Etat_Mouvement = Mouvement_non;
						return 0;
					}
					if(*p == 'A')
					{
						p++;
						if(*p == ':')
						{
							p++;
							cmd_c.Angle = calculAngle(p);
							cmd_c.Etat_Mouvement = Depl_Coord;
							return 1;
						}
					}
					else
					{
						cmd_c.Etat_Mouvement = Mouvement_non;
						return 0;
					}
				}
				else
					{
						cmd_c.Etat_Mouvement = Mouvement_non;
						return 0;
					}
				break; // fin case G
					
			case 'M':    
				
				p++;
        if(*p == 'O')
        {
          p++;
          if(*p == 'B')   
          { 
						if(*p != '\r') p++;
						if(*p != '\r') p++;
            if(*p == '\r')//pas de parametres
            {
              cmd_c.Etat_DCT_Obst = oui_360;
							cmd_c.DCT_Obst_Resolution = 30;
							return 1;
            }
            else if(*p == 'D')
						{
							cmd_c.Etat_DCT_Obst = oui_180;
							if(*p != '\r') p++;
							if(*p != '\r') p++;
							if(*p == 'A')
							{
								p++; // :
								if(*p == ':')
								{
									p++;
									// Traitement angle
									cmd_c.DCT_Obst_Resolution = calculAngleDCTObst(p);
									if(cmd_c.DCT_Obst_Resolution == 0)
									{	
										cmd_c.Etat_DCT_Obst = DCT_non;
										return 0;
									}
									else{
										return 1;
									}
								}
								else
								{
									cmd_c.Etat_DCT_Obst = DCT_non;
									return 0;
								}
							}
							if(*p == '\r')
							{
								cmd_c.DCT_Obst_Resolution = 30;
								return 1;
							}
						}
						else if(*p == 'A')
            {
								p++; // :
								if(*p == ':')
								{
									p++;
									// Traitement angle
									cmd_c.DCT_Obst_Resolution = calculAngleDCTObst(p);
									if(cmd_c.DCT_Obst_Resolution == 0) 
									{
										cmd_c.Etat_DCT_Obst = DCT_non;
										return 0;
									}
									else{
										cmd_c.Etat_DCT_Obst = oui_360;
										return 1;
									}
								}
								else
								{
									cmd_c.Etat_DCT_Obst = DCT_non;
									return 0;
								}
						}
						else
						{
							cmd_c.Etat_DCT_Obst = DCT_non;
							return 0;
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
							cmd_c.Etat_Lumiere = Allumer;
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
							return 1;
						}
					}
					else if(*p == '\r')
					{
						cmd_c.Etat_Lumiere = Allumer;
						cmd_c.Lumiere_Intensite = 100;
						cmd_c.Lumiere_Duree = 99;
						cmd_c.Lumiere_Extinction = 0;
						cmd_c.Lumiere_Nbre = 1;
						return 1;
					}
					else if(*p == 'S')
					{
						p++;
						if(*p == '\r')
						{
							cmd_c.Etat_Lumiere = Eteindre;
							return 1;
						}
					}
					else
					{
						return 0;
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
								cmd_c.Etat_Servo = Servo_C;
								break;
							case 'H':
								cmd_c.Etat_Servo = Servo_H;
								break;
							case 'V':
								cmd_c.Etat_Servo = Servo_V;
								break;
							default:
								cmd_c.Etat_Servo = Servo_non;
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
								cmd_c.Servo_Angle = calculAngleServo(p);
							}
							else
							{
								cmd_c.Etat_Servo = Servo_non;
								cmd_c.Servo_Angle = 0;
								return 0;
							}
						}								
					}
					return 1;
					break;
					
				default:
					return 0;
					break;
				
				
			} // fin switch
		return 0;
	} // while
		return 0;
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
					
	if(atoi(vitesse) >= 10 && atoi(vitesse) <= 100)
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
					
	if(atoi(angle) >= -90 && atoi(angle) <= 90)
		return atoi(angle);
	else
		return 0;

}

int calculAngleDCTObst(char* p)
{
	char xdata angle[6];
	
	int xdata i = 0;
	
	while((*p >= '0' && *p <= '9'))
	{
		angle[i] = *p;
		i++;
		p++;
	}
	angle[i]='\0';
	if(atoi(angle) >= 5 && atoi(angle) <= 45 && atoi(angle)%5 == 0)
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
			cmd_c.Coord_X = atoi(coord);
		else if(atoi(coord) >= -99 && atoi(coord) <0)
			cmd_c.Coord_X = atoi(coord) + 256;
		else
			cmd_c.Coord_X = 0;
	}
	else if(c == 'Y')
	{
		if(atoi(coord) >= - 99 && atoi(coord) <= 99)
			cmd_c.Coord_Y = atoi(coord);
		else if(atoi(coord) >= -99 && atoi(coord) <0)
			cmd_c.Coord_Y = atoi(coord) + 256;
		else
			cmd_c.Coord_Y = 0;
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
		cmd_c.Lumiere_Intensite = atoi(intensite);
	}
	else
	{
		cmd_c.Lumiere_Intensite = 100;
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
		cmd_c.Lumiere_Duree = atoi(duree);
	}
	else
	{
		cmd_c.Lumiere_Duree = 99;
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
		cmd_c.Lumiere_Extinction = atoi(duree);
	}
	else
	{
		cmd_c.Lumiere_Extinction = 0;
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
		cmd_c.Lumiere_Nbre = atoi(nombre);
	}
	else
	{
		cmd_c.Lumiere_Nbre = 1;
	}
}

char* Invite_Commandes(int epreuve) //message signalant que le robot est pret
{
	char xdata txt[40] = "txt";
	char *chaine;
	
	sprintf(txt,"I- Epreuve %d - Le Robot est pret",epreuve);
	
	// return txt : ne fonctionne pas ici, p-ê un probleme de mémoire. Utilisation de malloc pour corriger.
	
	chaine = malloc(strlen(txt)* sizeof(char));		// A faire pour eviter un probleme de mémoire
	strcpy(chaine,txt);
	
	return chaine;

}

char* Arrivee_point()//message signalant que le robot est arrivée au pointspécifie
{
	return "Arrivee au point specifie"; 
}

char* Servomoteur_positionne(char* b)//message signalant que le servomoteur a atteint sa position de consigne
{
	 char *chaine = malloc(5*sizeof(char));
	 strcpy(chaine,"AS ");
	 strcat(chaine,b);
   return chaine;
}
char* Detection_obstacle(BYTE *angle,unsigned int *distance, unsigned char pas)
{
	char i=0;
	char *txt = malloc(10*sizeof(char));
	char Angle;
	
	msg_info = malloc((pas*8 +4)* sizeof(char));
	if(msg_info == 0 || txt == 0)
	{
		return "malloc echec msg info";
	}
	else
	{
	strcpy(msg_info,"KOB ");
	for(i=0;i<pas;i++){
		if(angle[i]>=128) Angle=angle[i] - 256;
		else Angle = angle[i];
		
		sprintf(txt,"%d:%d ",(int)Angle,distance[i]);
		strcat(msg_info,txt);
  }
	strcat(msg_info, " \r");
	return msg_info;
	}
}
/*char* Detection_obstacle(BYTE *angle,unsigned int *distance, unsigned char pas)
{
	char i=0;
  char xdata chaine[100] = "KOB " ;
	char txt[10];
	char Angle;
	
	for(i=0;i<pas;i++){
		if(angle[i]>=128) Angle=angle[i] - 256;
		else Angle = angle[i];
		
		sprintf(txt,"%d: %d ",(int)Angle,distance[i]);
		strcat(chaine,txt);
  }
	strcat(chaine, " \r");
	return chaine;
}*/
char* Detection_obstacle_360(BYTE *angle, unsigned int *distanceAV, unsigned int *distanceAR, unsigned char pas)
{
	/* Affiche les mesures des capteurs Avant et Arrière (AV et AR)
		Les angles affichés sont compris entre -180° et +180. 
		AV balaye de -90° à 90° en passant par 0°
		AR balaye de 90° à -90 en passant par 180°
	*/
	char i=0;
	char txt[10];
	char Angle;
	
	msg_info = malloc((pas*8*2 +4)* sizeof(char));
	strcpy(msg_info,"KOB ");
	
	for(i=0;i<pas*2;i++){
		if(i<pas){
			if(angle[i]>=128) Angle=angle[i] - 256;
			else Angle = angle[i];
			
			sprintf(txt,"%d:%d ",(int)Angle,distanceAV[i]);
		}
		else{	
																						// **** !!!!!!! CONVERSION A REVOIR !!!!!!!!! ***
			if(angle[i-pas]>=128) Angle=angle[i-pas] - 256;
		else Angle = angle[i-pas];
			/*if(Angle >= -90 && Angle<= 0)	// Calcule de l'angle du capteur AR
					Angle += 180;
			else
					Angle -=180;*/
			
			sprintf(txt,"%d: %d ",(int)Angle,distanceAR[i-pas]);
		}
		strcat(msg_info,txt);
	}
	strcat(msg_info, " \r");
	return msg_info;
}

/*char* Detection_obstacle_360(BYTE *angle, unsigned int *distanceAV, unsigned int *distanceAR)
{
	/* Affiche les mesures des capteurs Avant et Arrière (AV et AR)
		Les angles affichés sont compris entre -180° et +180. 
		AV balaye de -90° à 90° en passant par 0°
		AR balaye de 90° à -90 en passant par 180°
	*//*
	char i=0;
  char xdata chaine[100] = "KOB " ;
	char txt[10];
	char Angle;
	
	for(i=0;i<12;i++){
		if(angle[i]>=128) Angle=angle[i] - 256;
		else Angle = angle[i];
		
		if(i<6){
			sprintf(txt,"%d: %d ",(int)Angle,distanceAV[i]);
		}
		else{																							// **** !!!!!!! CONVERSION A REVOIR !!!!!!!!! ***
			if(Angle >= -90 && Angle<= 0)	// Calcule de l'angle du capteur AR
					Angle += 180;
			else
					Angle -=180;
			
			sprintf(txt,"%d: %d ",(int)Angle,distanceAR[i-6]);
		}
		strcat(chaine,txt);
	}
	strcat(chaine, " \r");
	return chaine;
}*/

char* Info_auxilliaire(char* a)//information Info_auxilliaire
{
		strcat(a," \r");
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
