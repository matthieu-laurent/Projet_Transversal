#include <LIB_BSE_Config_Global.h>
#include <commandes_info_centrale.h>
#include <commandes_info_serilizer.h>
#include <intrins.h>
#include <stdio.h>


OUT_M1 cmd_central; // cmd : commande
IN_M1 info_central; // info : information
OUT_M2 cmd_serilizer;
IN_M2 info_serilizer;

void main(){
	
	Init_Device();
	
	// Variables initialisées à la valeur par défaut, cad, la 1ère valeur de l'enum ou 0 si c'est un entier.
	// Toutes les valeurs par défaut ne lance aucune action.
	
	/* ****
		Scrutation cmd :
			Période de structation: TIMER ?? INTERRUPTION ??
			Récupère la cmd via Getchar() 
			Analyse la cmd via Analyse_string
	
		Si CMD_OK alors Traiter la cmd.
	
		Traiter cmd:
			Lire variables (cmd_central, info_central, etc.) dans un ordre logique.
			Appeler les fonctions correspondantes.
	
		Arrêt via cmd:
			Si commande lue = Arrêt d'urgence alors lancer Arret_urgent()
	
			Arret_urgent(): Désactive tout via les fonctions de commandes moteurs, etc.
	
	***** */
}
