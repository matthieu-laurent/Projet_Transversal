#include <LIB_BSE_Config_Global.h>
#include <intrins.h>
#include <stdio.h>

// **** ultrason.h **************************
#define ULTRASON_COEF 59 // distance = durée_mesurée*340.29 (m/s) / 2 <=> distance = 0,017015 * durée_mesurée = durée_mesurée/59
#define ULTRASON_DMIN 2 // 2cm
#define ULTRASON_DMAX 400	// 400 cm
#define ULTRASON_TMIN  1*ULTRASON_COEF*ULTRASON_DMIN	// durée minimale mesurée pour laquelle la mesure est correcte (2 cm)
#define ULTRASON_TMAX  1*ULTRASON_COEF*ULTRASON_DMAX	// durée maximale mesurée pour laquelle la mesure est correcte (400 cm)

sfr16 DPTR=0x82;
sbit trig=P2^0; // P2.0 en d:0xa0
sbit INT0=P2^1; // crossbar à faire avec wizard
// **** fin ultrason.h **************************

/*
P2MDOUT   = 0x7B; // Configuration des bits du port 2
P3MDOUT   = 0xFC; // Configuration des bits du port 3
	
Les 3 lignes suivantes permettent d'activer le crossbar et de router INT1 sur P1.2
	  XBR0      = 0x07;
    XBR1      = 0x10;
		
EX0 = 1;		
*/
void envoi_pulsation(void) //fonction permettant de générer un delai de 10us pour declencher le trigger
{
	trig=1;
	_nop_(); //1
	_nop_(); //2
	_nop_(); //3
	_nop_(); //4
	_nop_(); //5
	_nop_(); //6
	_nop_(); //7
	_nop_(); //8
	_nop_(); //9
	_nop_(); //10
	trig=0;
}

unsigned short calcul_distance(void)
{
	unsigned short distance;
	envoi_pulsation();
	
	while(!INT0); //permet de generer un delai de 40ms
	TH0=0x00; // mise à 0 Timer0, démarrage mesure durée impulsion Echo du capteur ultrason.
	TL0=0x00; // TL0 en D:0x8a et TH0 en D:0x8c
	while(INT0) // INT0 = P2 en 0xA0
	{
		DPH=TH0;
		DPL=TL0;
	}

	if(DPTR>=ULTRASON_TMIN)  // Si durée mesurée correcte, obstacle > 2cm
	{
		if(DPTR<=ULTRASON_TMAX) // Si durée mesurée correcte, obstacle < 400cm
		{
			distance=DPTR/ULTRASON_COEF; // calcule la distance mesurée
		}
		else // sinon, obstacle > 400cm
		{
			distance = ULTRASON_DMAX; // distance = 400 cm : on peut avancer de 400 cm.
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
	unsigned short distance_cible=0;
	
	Init_Device();
	
	// Init Timer 0
	TMOD|=0x09; // timer0 en mode 2 (16bit Counter/timer)
	TMOD&=0xF9; // timer0 en mode 2 (16bit Counter/timer)
	CKCON|=0x08; // system clock
	IT0 = 1; // INT0 is edge triggered
	TR0=1; // mise en marche du timer0
	
	
	while(1) // boucle de test
	{
		distance_cible=calcul_distance();
		printf("%d\n", distance_cible);
		delay(20000);
	}
}
