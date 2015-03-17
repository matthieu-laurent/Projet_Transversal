//*****************************************************************************************************************************************
// Fonction d'initialisation
//*****************************************************************************************************************************************
IN_M2 S_INPUTS;    
OUT_M2 S_OUTPUTS;

void Init_Commande_Serializer()
{
	S_INPUTS.Etat_Commande=Commande_non;
    S_INPUTS.Ticks_mot1=0;               // paramètre Distance - Encoder Ticks moteur 1 (digo
    S_INPUTS.Vitesse_Mot1=0;             // paramètre de vitesse de rotation du moteur 1  (mogo digo
    S_INPUTS.Ticks_mot2=0;               // paramètre Distance - Encoder Ticks moteur 2 (digo
    S_INPUTS.Vitesse_Mot2=0;
    S_OUTPUTS.Etat_Response=Reponse_non;
}

void Avancer(int vitesse)				// vitesse=45 => avance à 45% de sa vitesse max
{
	S_INPUTS.Vitesse_Mot1=vitesse;
	S_INPUTS.Vitesse_Mot2=vitesse;
	S_INPUTS.Etat_Commande=mogo_1_2;
}

void Reculer(int vitesse)				// vitesse=45 => reculer à 45% de sa vitesse max
{
	S_INPUTS.Vitesse_Mot1=-vitesse;
	S_INPUTS.Vitesse_Mot2=-vitesse;
	S_INPUTS.Etat_Commande=mogo_1_2;
}

void Stop()		
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

void Rotation_180(int vitesse)
{
	S_INPUTS.Ticks_mot1=1972;
	S_INPUTS.Vitesse_Mot1=vitesse;
	S_INPUTS.Etat_Commande=digo_1;	
}

void Rotation_angle(int angle,int vitesse)
{
	S_INPUTS.Ticks_mot1=angle*10.9389;
	S_INPUTS.Vitesse_Mot1=vitesse;
	S_INPUTS.Etat_Commande=digo_1;	
}
