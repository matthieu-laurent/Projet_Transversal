//------------------------------------------------------------------------------------
// main.c
//------------------------------------------------------------------------------------
// AUTH: Groupe A1
// DATE: 10/03/2015
//
// Target: C8051F020
// Tool chain: KEIL Eval 'c'
//
//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------//
#ifndef config_global
#define config_global
#include "config_global.h"
#endif	// Configuration globales du µprocesseur

#include "config_peripheriques.h"
//------------------------------------------------------------------------------------
// Global CONSTANTS
//------------------------------------------------------------------------------------
unsigned char Intensite;
unsigned char Lum_ON;
unsigned char Lum_OFF;
unsigned char Lum_Nbre;
unsigned int cpt;
unsigned int nbOverflow;
unsigned char ON;
//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
void main (void) {	
	Intensite=80; // en %
	Lum_ON=20;// choisir valeur entre 1 et 100
	Lum_OFF=10;// choisir valeur entre 1 et 100
	Lum_Nbre=10;
	cpt=0;
	ON=1;
	nbOverflow=Lum_ON*4; // (x*10^-1)/(25*10^-3) rappel: un overflow tt les 25 ms
	Init_Device();
	PCA_Init();
	EA=1; // Autorisation interruptions
	Lumiere();
	while (1) { // Boucle infinie
		
		
		
		
		
		switch(){
			case :
				
				break;
		}
			
		
		
	}
}

typedef struct COMMANDES // Cette structure contient toutes les commandes envoyées par la centrale de commande
// Les variables de type enum correspondent aux commandes
// Les variables de type BYTE, UNSIGNED_BYTE ou int correspondent aux paramètres
{
enum Epreuve Etat_Epreuve; //Contient le numéro d'épreuve en cours - Commandes "D" "E" "Q"
enum Mouvement Etat_Mouvement; //Contient le type de mouvement demandé - Commandes "A" "B" "S" "RD" "RG" "RC" "RA" "G"
// Paramètres des commandes précédentes:
BYTE Vitesse; // Paramètre vitesse - Peut aussi être modifié par la commande "TV"
BYTE Coord_X; // Paramètre coordonnée X
BYTE Coord_Y; // Paramètre coordonnée Y
int Angle; // Paramètre angle
enum ACQ_Son Etat_ACQ_Son; // Contient la commande d'acquisition de signaux sonores "ASS"
// Paramètre de la commande précédente:
BYTE ACQ_Duree; // Paramètre durée de la séquence d'acquisition
enum DCT_Obst Etat_DCT_Obst; // Contient la commande de détection d'obstacle "MOB D" "MOB"
// Paramètre de la commandes précédente:
BYTE DCT_Obst_Resolution; // Paramètre résolution angulaire de la détection d'obstacle
enum Lumiere Etat_Lumiere; // Contient la commande d'allumage du pointeur lumineux "L" "LS"
// Paramètres de la commande précédente:
BYTE Lumiere_Intensite; // Paramètre intensité lumineuse
BYTE Lumiere_Duree; // Paramètre durée d'allumage
BYTE Lumiere_Extinction; // Paramètre durée d'extinction
BYTE Lumiere_Nbre; // Paramètre nombre de cycles d'allumage/extinction
enum Servo Etat_Servo; // Contient la commande de positionnement d'un servo "CS C" "CS H" "CS V"
// Paramètre de la commande précédente:
BYTE Servo_Angle; //Paramètre angle de rotation
} OUT_M1;
