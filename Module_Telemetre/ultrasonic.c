#include <LIB_BSE_Config_Global.h>
#include <intrins.h>
#include <stdio.h>

// **** ultrason.h **************************
#define ULTRASON_COEF 58 // distance = durée_mesurée*340.29 (m/s) / 2 <=> distance = 0,017015 * durée_mesurée = durée_mesurée/59
#define ULTRASON_DMIN 2 // 2cm
#define ULTRASON_DMAX 400	// 400 cm
#define ULTRASON_TMIN  1*ULTRASON_COEF*ULTRASON_DMIN	// durée minimale mesurée pour laquelle la mesure est correcte (2 cm)
#define ULTRASON_TMAX  1*ULTRASON_COEF*ULTRASON_DMAX	// durée maximale mesurée pour laquelle la mesure est correcte (400 cm)

sbit trig=P3^4; // P2.0 en d:0xa0
sbit INT0=P3^5; // crossbar à faire avec wizard ECHO
int nb_debordement = 0;
// **** fin ultrason.h **************************


void envoi_pulsation(void) //fonction permettant de générer un delai de 10us pour declencher le trigger
{
	int i;
	trig=1;
	for(i=0;i<223;i++) // Pour le delai de 10us
	{
		_nop_(); 
	}

	trig=0;
}

void  ISR_Timer0 (void) interrupt 1	
{
	//	TF0 cleared by hardware (voir table des interrutptions): Se met à 0 automatiquement 
	nb_debordement++;	// Compte le nombre de débordements pour Ultrason
}

unsigned int MES_Dist_AV(void)
{
	/* *** Tâche: Mesure la distance entre robot et obstacle, par le capteur avant. ***
	Fonctionnement Matériel:
		Envoyer une pulsation de 10µs au capteur, celui-ci renvoie un signal (Echo) d'une durée proportionnelle
		à la distance entre le capteur et l'obstacle.
		Calcul: Distance(cm) = Temps_mesuré(µs)/58. 
	****************************************
	Explication de la fonction:
			Envoie une pulsation au capteur (Trigger), via envoi_pulsation()
			Attend la récéption du signal réponse du capteur (Echo).
			Initialise à 0 et Démarre le Timer0. ** Voir si utiliser GATE0 (pour démarrer quand TR0=1 et INT0=1) améliore la précision.
			Attend la fin du signal Echo.
			Récupère la valeur du timer dans "a". 
				Afin de convertir "a" en µs: 
					Prend en compte les débordements du Timer0.
					Divise "a" par 22 :  Sysclock (22 Mhz)= 22*clock_1µs (1 Mhz).
			Calcule la distance à renvoyer:
				Si distance conforme (entre 2cm et 400cm) on renvoie a/58 afin de convertir en cm.
				Sinon on renvoie 0.
*********************************************************************		*/
	
	unsigned int distance;
	unsigned long int a;	// **********************!!!!!!!!!!!!!!!!! CHANGER TYPE mettre sur 32 bits! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//unsigned int usmax = ULTRASON_TMAX; tester sans usmax d'abord
	
	nb_debordement = 0;
	
	envoi_pulsation();

	while(!INT0); //	 Attend le signal Echo
	TH0=0x00; // Mise à 0 Timer0
	TL0=0x00; // Rq: TL0 en D:0x8a et TH0 en D:0x8c
	TR0=1; // Démarre la mesure de durée d'impulsion Echo du capteur ultrason.
	
	while(INT0);	// Attend fin signal Echo
	
	TR0=0; // Arrêt du Timer0
	a=0;
	a=TH0;	// Récupère la valeur du Timer0, ces 3 lignes fonctionnent bien.
	a=a<<8;
	a|=TL0;
	
	if(nb_debordement>0)
	{
		a=a+nb_debordement*65535; // Prend en compte les débordements , déborde dans ISR_Timer0()
	}
	a=a/22;	// Convertie en µs : Sysclock (22 Mhz)= 22*clock_1µs (1 Mhz)

	if(a>=ULTRASON_TMIN)  // Si durée mesurée correcte, obstacle > 2cm
	{
		if(a<=ULTRASON_TMAX) // Si durée mesurée correcte, obstacle < 400cm
		{
			distance= a/58; // calcule la distance mesurée
		}
		else // sinon, obstacle > 400cm
		{
			distance = 0; // Imposé par le cahier des charges
		}
	}
	else
	{
		distance = 0; // Obstacle juste devant le capteur (à moins de 2cm)
	}
		 
	return distance;
}

void delay(unsigned short duree) // delai logiciel pour tester
{
	unsigned short i;
	
	for(i=0;i<duree;i++)
	{
		_nop_();
	}
}

void main()
{
	unsigned int distance_cible=0;
	
	trig = 0;
	Init_Device();
	
	// Init Timer 0
	TMOD=0x01; // timer0 en mode 1 (16bit Counter/timer)
	CKCON|=0x08; // system clock
	// IT0 = 1; // INT0 is edge triggered
	ET0 = 1;	// Active Interruption Timer0
	

	while(1) // boucle de test
	{
		distance_cible=MES_Dist_AV();
		printf("%d\n", distance_cible);
		delay(10000); // délai correct, ne pas mettre plus. Chercher en quoi ce délai perturbe le test.
	}
}
