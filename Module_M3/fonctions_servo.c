#include <fonctions_servo.h>
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
Operating Speed (4.8V): 0.19sec/60° at no load
Operating Speed (6.0V): 0.15sec/60° at no load
*****************************************************************************************************************/

// !!!!!!!!!!!!!!! inclure le ".h" du module M1 +++++ enum +++ faire des if !!!!!!!!!!!!!!


/* variable globale */
char Angle_actuel=0;

unsigned char CDE_Servo_V(char Angle){
	double codeDec;
	
	int duree=0; // temps pour que le servomoteur passe de la position "Angle_actuel" à la position final "Angle"

	Angle_actuel=Angle; // Mise à jour angle
	
	
	
	codeDec= 17.656*Angle + 62706;
	
	PCA0CPL2  = ((int)codeDec)%256;
  PCA0CPH2  = (int)(codeDec/256);
	
	// Estimation durée
	// cas 1 : Operating Speed (4.8V): 0.19sec/60° at no load
	if(Angle>Angle_actuel)
		duree=0,19*(Angle-Angle_actuel)/60; // unité duree: sec
	else 
		duree=0,19*(Angle_actuel-Angle)/60;
	/*// cas 2 : Operating Speed (6.0V): 0.15sec/60° at no load
	if(Angle>Angle_actuel)
		duree=0,15*(Angle-Angle_actuel)/60; // unité duree: sec
	else 
		duree=0,15*(Angle_actuel-Angle)/60; */
	
	duree=duree*100; // duree en centième de seconde 
	return (char)duree;

}
/*
unsigned char CDE_Servo_H (char Angle){
	return Angle;
} */
