#include "Gestion_Globale_Master.h"



OUT_M1 xdata cmd_c; // cmd_c : commande centrale
IN_M1 xdata info_c; // info : information
OUT_M2 xdata S_OUTPUTS;  // S_ : serializer
IN_M2 xdata S_INPUTS; 

unsigned int xdata distance_cible=0;
bit start = 0;
bit envoyer_info=0;
bit FLAG_ECRITURE = 1; 					// UART1
bit   TXactive = 0;             // UART0 transmission status flag (off) 
int xdata longueur_cmd = 0;
char var_test=0;
char  xdata outbuf[MAX_BUFLEN];     // memory for ring buffer #1 (TXD) //buffer UART0
char  xdata inbuf [MAX_BUFLEN];     // memory for ring buffer #2 (RXD)

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
	bool I_epreuve[9] = {0,1,1,1,1,1,1,1,1};
	
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
	
	while(1){
	
		
		if(start){
			start = 0;
			id_cmd = 0;
		do{ 
					value = serInchar();
					if(value != 0)
					{
						cmd[id_cmd] = value;
						id_cmd++;
					}
			}while((value != 0) && (value != '\r'));				
				
		if(value == '\r')	// si une commande à été saisie
		{		
			//printf("cmd = %s",cmd);
			// analyse cmd
			//Analyse_String("A 20\r");
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
						if(I_epreuve[epreuve1]){
							serOutstring(Invite_Commandes());
							I_epreuve[epreuve1] = 0;
						}
					break;
					
					case epreuve2:
					break;
					case epreuve3:
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
			
			}			
		}
		}
  }

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
		}
		else{	// Si buffer FULL
			if(start == 0){ 	// Si on a pas de commande à traiter en cours
				for(i=0;i<longueur_cmd;i++){	// "Vide" le buffer, on considère que la commande est érroné donc on l'ignore. 
					RB_POPADVANCE(&in);
				}
				longueur_cmd = 0;
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
		default: break;
	
	}

}

void Gestion_DCT_Obst(void){
	char i=0;
	unsigned int mesures[6], mesuresAR[6];
	BYTE angles[6];
	
	cmd_c.Etat_Servo = Servo_H;	// Servomoteur pour piloter les capteurs ultrasonics
	
	switch(cmd_c.Etat_DCT_Obst){
		case oui_180: 
			cmd_c.Servo_Angle = -75;	// angle de mesure = 30°, on place le servo à -75 pour couvrir -90 -> -60
			for(i=0;i<6;i++){				// Effectue un balayage de 180°. Mesure la distance d'obstacle tous les 30° (6fois). 
				//CDE_Servo();
				delai_us(5000); // attend 0.5secondes
				mesures[i] = MES_Dist_AV();		
				//mesures[i] = i*10;		
				angles[i] = cmd_c.Servo_Angle;
				cmd_c.Servo_Angle+=30;	// puis on augmente l'angle de 30° pour couvrir -60 -> -30 et ainsi de suite
			}
			
			serOutstring(Detection_obstacle(angles,mesures));	// Envoie message information
			break;
		
		case oui_360:				// Idem mais avec les 2 capteurs ultrasonics
			cmd_c.Servo_Angle = -75;
			for(i=0;i<6;i++){
				CDE_Servo();
				delai_us(5); // attend 0.5secondes
			//	mesures[i] = MES_Dist_AV();		
			// mesuresAR[i] = MES_Dist_AR();
				mesures[i] = i*10;		
				mesuresAR[i] = i*20;						
				angles[i] = cmd_c.Servo_Angle;
				
				cmd_c.Servo_Angle+=30;
			}
			
			serOutstring(Detection_obstacle_360(angles,mesures,mesuresAR));	// Envoie message information
			break;
		
		default: break;
	}
	
	cmd_c.Etat_Servo = Servo_non;		// Afin de réinitialiser la variable
	
}

void Gestion_Servo(void){

		char txt[1] = 'H'; 
	switch(cmd_c.Etat_Servo){
		case Servo_H: 
			CDE_Servo();
			txt[0] = 'H';
			serOutstring(Servomoteur_positionne(txt));
			break;
		
		case Servo_V:
			CDE_Servo();
			txt[0] = 'V';
			serOutstring(Servomoteur_positionne(txt));
			break;
		
		case Servo_C:
			CDE_Servo();
			txt[0] = 'V';
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
