#include "servo.h"
#include "commandes_info_centrale.h"
#include "LIB_BSE_Config_Global.h"
#include <string.h>
#include <math.h>


/***************************************************************************************************************
Sous ensemble FO-M3: Commande des servomoteurs

Fichiers associer: config_global.c/h + fonctions_servo.h + init_pca.c/h + main.c

Fonctionnement:
1) Le signal de commande du servomoteur sera générer par le PCA du 8051 et correspond à un signal PWM
dont la largeur d'impulsion est proportionelle à l'angle que l'on veut imposer au servomoteur
2) Datasheet du servomoteur: 600us <=> +90°	
														 1500us <=> 0°
														 2400us <=> -90°
3) Pour modifier la largeur d'impulsion, et donc l'angle désiré du servomoteur, nous devons modifier 
les registres suivants: PCA0CPL2 et PCA0CPH2  
4) 600us <=> (FB22)h=(64290)d
	 1500us <=> (F4FD)h <=> (62717)d
	 2400us <=> (EEB8)h <=> (61112)d
5) A l'aide de excel, nous avons determiné la fonction suivante: codeDec= 17,656*Angle + 62706
6) vitesse de rotation:
Operating Speed (4.8V): 0.22sec/60° at no load
Operating Speed (6.0V): 0.18sec/60° at no load

7) commande angle (récupérée en hexa):	0<=> 0 ; 127 <=> 127 <=> 127 ; 128 <=> -128 ; 255 <=> -1

	
*****************************************************************************************************************/

extern OUT_M1 xdata cmd_c;

/* variable globale */
char Angle_actuel_V=0;
char Angle_actuel_H=0;
char Angle_actuel_C=0;



unsigned long int CDE_Servo(){
	unsigned char Angle1=cmd_c.Servo_Angle;
	char Angle=0;
	double codeDec;
	double duree=0; // temps pour que le servomoteur passe de la position "Angle_actuel" à la position final "Angle"
	
	
	if(Angle1>=128) Angle=Angle1 - 256;
	else Angle = Angle1;
	
	if((Angle_actuel_C==Angle&&cmd_c.Etat_Servo==Servo_C)||(Angle_actuel_H==Angle&&cmd_c.Etat_Servo==Servo_H)||(Angle_actuel_V==Angle&&cmd_c.Etat_Servo==Servo_V))
	{
		return 0;
	}	
	else{
	
		codeDec= 17.656*Angle + 62706;
		
		if(cmd_c.Etat_Servo==Servo_non)
		{
			return 0;
		}
		
		if(cmd_c.Etat_Servo==Servo_C)
		{
			// Estimation durée
			// cas 1 : Operating Speed (4.8V): 0.19sec/60° at no load
			if(Angle>Angle_actuel_C)
				duree=(double)(0,22*(Angle-Angle_actuel_C)/60); // unité duree: sec
			else 
				duree=(double)(0,22*(Angle_actuel_C-Angle)/60);
			/*// cas 2 : Operating Speed (6.0V): 0.15sec/60° at no load
			if(Angle>Angle_actuel_C)
				duree=0,15*(Angle-Angle_actuel_C)/60; // unité duree: sec
			else 
				duree=0,15*(Angle_actuel-Angle_C)/60; */
			
			duree=duree*100; // duree en centième de seconde 
			PCA0CPL0  = ((int)codeDec)%256;
			PCA0CPH0  = (int)(codeDec/256);
			
			Angle_actuel_C=Angle; // MaJ Angle actuel
		}
		
		if(cmd_c.Etat_Servo==Servo_H)
		{
		
			if(Angle>Angle_actuel_H)
				duree=0,22*(Angle-Angle_actuel_H)/60; // unité duree: sec
			else 
				duree=0,22*(Angle_actuel_H-Angle)/60;
		
			
			duree=duree*100; // duree en centième de seconde 
			
			PCA0CPL1  = ((int)codeDec)%256;
			PCA0CPH1  = (int)(codeDec/256);
			
			Angle_actuel_H=Angle; // MaJ Angle actuel
		}
		
		if(cmd_c.Etat_Servo==Servo_V)
		{
			if(Angle>Angle_actuel_V)
				duree=0,22*(Angle-Angle_actuel_V)/60; // unité duree: sec
			else 
				duree=0,22*(Angle_actuel_V-Angle)/60;
			
			duree=duree*100;
			PCA0CPL2  = ((int)codeDec)%256;
			PCA0CPH2  = (int)(codeDec/256);
			
			Angle_actuel_V=Angle; // MaJ Angle actuel
		}
		
		return (unsigned long int)duree;
	}
}
