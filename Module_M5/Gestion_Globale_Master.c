#include "Gestion_Globale_Master.h"

jmp_buf env;
int error_jmp=0;

OUT_M1 xdata cmd_c; // cmd_c : commande centrale
IN_M1 xdata info_c; // info : information
OUT_M2 xdata S_OUTPUTS;  // S_ : serializer
IN_M2 xdata S_INPUTS; 

unsigned int xdata distance_cible=0;
bit ARRET_URGENCE = 0;
bit erreur_U0_RX = 0;
bit start = 0;
bit envoyer_info=0;
bit FLAG_ECRITURE = 1; 					// UART1
bit   TXactive = 0;             // UART0 transmission status flag (off) 
int xdata longueur_cmd = 0;
char var_test=0;
char  xdata outbuf[MAX_BUFLEN];     // memory for ring buffer #1 (TXD) //buffer UART0
char  xdata inbuf [MAX_BUFLEN];     // memory for ring buffer #2 (RXD)
extern char xdata *msg_info;

// buffer UART1
char xdata buf_rx1[MAX_BUFLEN];
char xdata rx1_rd = 0;
char xdata rx1_wr = 0;

// Créer les structures out et in (struct rb)
RB_CREATE(out,unsigned char xdata);            // static struct { ... } out; 
RB_CREATE(in, unsigned char xdata);            // static struct { ... } in; 


void main(){
	unsigned char   xdata  value, rc = 0, id_cmd=0;
	unsigned char xdata cmd[30] = "";
	int xdata check = 0;
	char c = 'd';
	char I_epreuve=0;
	
	Init_Device();

	//cmd_c.Etat_Epreuve = epreuve1;
	
	/* Variables initialisées à la valeur par défaut, cad, la 1ère valeur de l'enum ou 0 si c'est un entier.
	// Toutes les valeurs par défaut ne lance aucune action.

	 ****
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

  serInit();   // Initialise les buffers UART0                         
 
	cmd_c.Etat_Epreuve = epreuve1; // pour test
	
	error_jmp = setjmp(env);
	
	if(ARRET_URGENCE)
	{
		RETI_ISR();
		Arret_Urgence_Fonction();
	}
	while(1){
	
		if(start){
			start = 0;
			id_cmd = 0;
			strcpy(cmd,"");
		do{ 		// Récupère la commande stockée dans le buffer
					value = serInchar();
					if(value != 0)
					{
						cmd[id_cmd] = value;
						id_cmd++;
					}
			}while((value != 0) && (value != '\r'));				
				
		if(value == '\r')	// si une commande à été saisie
		{		
			// Analyse la commande et affecte les variables de "cmd_c." correspondantes
			check = Analyse_String(cmd);
			if(check == 1)
			{
				switch(cmd_c.Etat_Epreuve){
					case epreuve1: 
						if(cmd_c.Etat_Mouvement != Mouvement_non){
							Gestion_Mouvement();
							Action_UART1();
							cmd_c.Etat_Mouvement = Mouvement_non;
							//for(tempo_mot=0;tempo_mot<1000000;tempo_mot++){}
						//	serOutstring("test\r\n");
						}
						if(cmd_c.Etat_DCT_Obst != DCT_non){
							Gestion_DCT_Obst();
							cmd_c.Etat_DCT_Obst = DCT_non; // lance une seule détection par commande reçue.
						}
						if(cmd_c.Etat_Servo != Servo_non){
							Gestion_Servo();			// Lance servo et affiche message information
							cmd_c.Etat_Servo = Servo_non;
						}
						if(cmd_c.Etat_Lumiere != Lumiere_non){
							Gestion_Lumiere();
							cmd_c.Etat_Lumiere = Lumiere_non;
							// Action_Lumiere()
						}
						if(cmd_c.Etat_ACQ_Son != ACQ_non){
							//Action_ACQ_Son()  car Etat_ACQ_Son = oui/non
							cmd_c.Etat_ACQ_Son = ACQ_non;
						}
						
						// Message Information Epreuve1:
						if(I_epreuve != 1){
							serOutstring(Invite_Commandes(1));
							I_epreuve = 1;
						}
					break;
					
					case epreuve2:
						if(cmd_c.Etat_Mouvement != Mouvement_non){
							Gestion_Mouvement();
							Action_UART1();
							cmd_c.Etat_Mouvement = Mouvement_non;
							//for(tempo_mot=0;tempo_mot<1000000;tempo_mot++){}
						//	serOutstring("test\r\n");
						}
						if(cmd_c.Etat_DCT_Obst != DCT_non){
							Gestion_DCT_Obst();
							cmd_c.Etat_DCT_Obst = DCT_non; // lance une seule détection par commande reçue.
						}
						if(cmd_c.Etat_Servo != Servo_non){
							Gestion_Servo();			// Lance servo et affiche message information
							cmd_c.Etat_Servo = Servo_non;
						}
						if(cmd_c.Etat_Lumiere != Lumiere_non){
							Gestion_Lumiere();
							cmd_c.Etat_Lumiere = Lumiere_non;
							// Action_Lumiere()
						}
						if(cmd_c.Etat_ACQ_Son != ACQ_non){
							//Action_ACQ_Son()  car Etat_ACQ_Son = oui/non
							cmd_c.Etat_ACQ_Son = ACQ_non;
						}
						if(I_epreuve != 2){
							serOutstring(Invite_Commandes(2));
							I_epreuve = 2;
						}
					break;
					case epreuve3:
						
						if(I_epreuve != 3){
							serOutstring(Invite_Commandes(3));
							I_epreuve = 3;
						}
					break;
					case Fin_Epreuve:	
						/* Suite à la commande "E" - Etat dans lequel on ne fait rien à part attendre 
							 la commande "D valeur" pour démarrer l'épreuve n°valeur
						*/
							if(I_epreuve != 9){
								I_epreuve = 9;
						}
							else
							{
								serOutstring("Action impossible - Demarrez une epreuve");
							}
							Reinit_cmd(); // Permet de ne pas prendre en compte les commandes envoyées quand on est en Fin_Epreuve (après la commande "E")
					break;
					default:
					break;
			}
			// Transmission messages vers PC:
			serOutstring(Accuse_Reception_OK());
		}
		else{	// commande invalide
			serOutstring(Accuse_Reception_KO());
		}
	}
		else{
			if(value == 0) //buffer vide : erreur ou ne fait rien
			{ 
				serOutstring("Erreur Buffer Vide U0 RX\r\n#");
			}			
		}
		}	// fin if(start)
		if(erreur_U0_RX){
			serOutstring("Erreur Buffer Plein U0 RX\r\n#");
			erreur_U0_RX = 0;
		}
  }	// fin while(1)

}

void serInit(void) {

  RB_INIT(&out, outbuf, MAX_BUFLEN-1);           /* set up TX ring buffer */
  RB_INIT(&in, inbuf,MAX_BUFLEN-1);             /* set up RX ring buffer */

	// Ajouter ici le code de l'initialisation et de configuration de l'UART0

}

void UART0_ISR(void) interrupt 0x4 {

	char car,i=0; 
	
  if (TI0==1) // On peut envoyer une nouvelle donn?e sur la liaison s?rie
  { 
  	if(!RB_EMPTY(&out)) {
       SBUF0 = *RB_POPSLOT(&out);      // start transmission of next byte 
       RB_POPADVANCE(&out);             // remove the sent byte from buffer 
  	}
  	else TXactive = 0;                    // TX finished, interface inactive 
	TI0 = 0;
  }
  else // RI0 à 1 - Donc une donn?e a ?t? re?ue
  {
    if(!RB_FULL(&in)) {
			car = SBUF0;
			longueur_cmd++;
			
     	*RB_PUSHSLOT(&in) = car;        // store new data in the buffer 
			RB_PUSHADVANCE(&in);               // next write location 

			if(car == '\r')
			{
				start = 1;
				longueur_cmd = 0;
			}
			else if(car == 'Q')
			{
				ARRET_URGENCE = 1;
				//EIP2 |= 0x40;
				RI0 = 0;
				longjmp(env,1);
			}
		}
		else{	// Si buffer FULL
			if(start == 0){ 	// Si on a pas de commande à traiter en cours
				for(i=0;i<longueur_cmd;i++){	// "Vide" le buffer, on considère que la commande est érroné donc on l'ignore. 
					RB_POPADVANCE(&in);
				}
				longueur_cmd = 0;
				erreur_U0_RX=1;
			}			
		}
   RI0 = 0;
  }
}

void UART1_ISR(void) interrupt 20{
	/* code précédent sans Macro: if(rx1_wr < MAX_BUFLEN){
				buf_rx1[rx1_wr] = SBUF1;
				
				if(buf_rx1[rx1_wr] == '\r'){
					// lire l'information reçu , envoyer à UART0
					envoyer_info=1;
				}
				
				rx1_wr++;
			}
			else rx1_wr = 0;	*/
	 char xdata c=0; 
	
	if((SCON1&0x02) == 0x02){ // Si TI1 = 1 : transimission
		FLAG_ECRITURE = 1;
		SCON1&=0xFD;	// TI1 = 0
	}
	else{
		if((SCON1&0x01) == 0x01){ // Sinon si RI1 = 1 : récéption
			c = SBUF1;
			serOutchar(c);	
			SCON1&=0xFE;	// RI1 = 0
		}
	}
}

void Arret_Urgence_Fonction(void){
				
	Arret();
	Action_UART1();
		//	EIP2 &= 0xBF;
	serOutstring("Arret d Urgence");
	serOutstring(Accuse_Reception_OK());
	Reinit_cmd();
	ARRET_URGENCE = 0;
}

void Gestion_Mouvement(void){
	switch(cmd_c.Etat_Mouvement){
		case Avancer:
				Avancer_m(cmd_c.Vitesse);
		break;
		case Reculer:
				Reculer_m(cmd_c.Vitesse);
		break;
		case Stopper:
				Arret();
		break;	
		case Rot_90D:
				Rotation_Droite_90(cmd_c.Vitesse);
		break;
		case Rot_90G:
				Rotation_Gauche_90(cmd_c.Vitesse);
		break;
		case Rot_180D:
				Rotation_180(cmd_c.Vitesse,'D');
		break;
		case Rot_180G:
				Rotation_180(cmd_c.Vitesse,'G');
		break;
		case Rot_AngD:
				Rotation_angle(cmd_c.Angle,cmd_c.Vitesse,'D');
		break;
		case Rot_AngG:
				Rotation_angle(cmd_c.Angle,cmd_c.Vitesse,'G');
		break;
		case Depl_Coord:
				Aller_en(cmd_c.Coord_X,cmd_c.Coord_Y,cmd_c.Angle,cmd_c.Vitesse);
		break;
		default: break;
	
	}

}

void Gestion_DCT_Obst(void){
	char i=0;
	unsigned char pas = 1 + (180/cmd_c.DCT_Obst_Resolution);
	/*unsigned int mesures[pas], mesuresAR[pas];
	BYTE angles[pas];*/
	
	unsigned int *mesures = malloc(pas* sizeof(unsigned int));
	unsigned int *mesuresAR = malloc(pas* sizeof(unsigned int));
	BYTE *angles = malloc(pas* sizeof(BYTE));
	
	if(mesures == 0)
		serOutstring("malloc mesures echec \r");
	else
	{
	cmd_c.Etat_Servo = Servo_C;	// Servomoteur pour piloter les capteurs ultrasonics
	
	switch(cmd_c.Etat_DCT_Obst){
		case oui_180: 
			cmd_c.Servo_Angle = -90;	// angle de mesure = 30°, on place le servo à -75 pour couvrir -90 -> -60
			for(i=0;i<pas;i++){				// Effectue un balayage de 180°. Mesure la distance d'obstacle tous les 30° (6fois). 
				CDE_Servo(); 
				delai_us(5); // delai_us(50000); attend 0.5secondes
				//mesures[i] = MES_Dist_AV();		
				//mesures[i] = i*10;		
				//((unsigned int*)mesures)[i] = i*10;		
				mesures[i] = i*10;		
				angles[i] = cmd_c.Servo_Angle;
				cmd_c.Servo_Angle+=cmd_c.DCT_Obst_Resolution;	// puis on augmente l'angle de 30° pour couvrir -60 -> -30 et ainsi de suite
			}
			//Detection_obstacle(angles,mesures,pas);
			//serOutstring_tst();
			serOutstring(Detection_obstacle(angles,mesures,pas));	// Envoie message information
			break;
		
		case oui_360:				// Idem mais avec les 2 capteurs ultrasonics
			cmd_c.Servo_Angle = -90;
			for(i=0;i<pas;i++){
				CDE_Servo();
				delai_us(50000); // attend 0.5secondes
			//	mesures[i] = MES_Dist_AV();		
			// mesuresAR[i] = MES_Dist_AR();
				mesures[i] = i*10;		
				mesuresAR[i] = i*20;						
				angles[i] = cmd_c.Servo_Angle;
				
				cmd_c.Servo_Angle+=cmd_c.DCT_Obst_Resolution;
			}
			
			serOutstring(Detection_obstacle_360(angles,mesures,mesuresAR,pas));	// Envoie message information
			break;
		
		default: break;
		}
	}
	free(mesures);
	free(mesuresAR);
	free(angles);
	free(msg_info);

	cmd_c.Etat_Servo = Servo_non;		// Afin de réinitialiser la variable

}

void Gestion_Servo(void){

		char *txt = 'H'; 
	switch(cmd_c.Etat_Servo){
		case Servo_H: 
			CDE_Servo();
			*txt = 'H';
			serOutstring(Servomoteur_positionne(txt));
			break;
		
		case Servo_V:
			CDE_Servo();
			*txt = 'V';
			serOutstring(Servomoteur_positionne(txt));
			break;
		
		case Servo_C:
			CDE_Servo();
			*txt = 'C';
			serOutstring(Servomoteur_positionne(txt));
			break;
		
		default: break;
	}
}

void Gestion_Lumiere(void){
	switch(cmd_c.Etat_Lumiere){
		case Allumer:
			break;
		case Eteindre:
			break;
		
		default:break;
	}
}

void Reinit_cmd(void){
	cmd_c.Etat_Mouvement = Mouvement_non;
	cmd_c.Etat_ACQ_Son = ACQ_non;
	cmd_c.Etat_DCT_Obst = DCT_non;
	cmd_c.Etat_Servo = Servo_non;
	cmd_c.Etat_Lumiere = Lumiere_non;
}
void delai_us(unsigned long int duree){
	unsigned long int i=0;
	int j=0;
	//int coef = COEF_NS_US;
	int coef = 1;
	
	for(j=0;j<coef;j++){
		for(i=0;i<duree;i++)
		{
			_nop_(); 
		}
	}
}

