/******************************************************************
*******************Projet Transversal******************************
*****************Module : Capteur a ultrasons**********************
***********************Version 1***********************************
**************Date de derniere modification : 05.03.2015***********
******************************************************************/*

#include <C8051F020.H>
#include <intrins.h> //permet d'utiliser les fonctions _nop_() pour la temporisation

sfr16 DPTR=0x82;
sbit trig=P3^5; //on declare une variable trig representant le trigger sur le 5eme bit de la broche P3^5

void envoi_pulsation(void) //fonction permettant de générer un delai de 10us pour declencher le trigger
{
	TH0=0x00;
	TL0=0x00;
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

unsigned char calcul_distance(void)
{
	unsigned char distance;
	envoi_pulsation();
	
	while(!INT0); //permet de generer un delai de 40ms
	while(INT0)
	{
		DPH=TH0;
		DPL=TL0;
	}
	TH0=0xFF;
	TL0=0xFF;
	if(DPTR<38000) //possible que le cpateur ne marche pas a ce niveau, dans ce cas prendre 35000
		distance=DPTR/58,771; //voir calcul, distance en cm
	else
		distance=0; //pas d'obstacle devant le capteur
}

void main()
{
	TMOD=0x09; // timer0 en mode 2 (16bit Counter/timer)
	TR0=1; // mise en marche du timer0
	TH0=0x00;
	TL0=0x00;
	P3|=0x04; // configuration du pin P3.5 pour en faire une entrée
	
	unsigned int distance_cible=0;
	while(1)
	{
		distance_cible=calcul_distance();
		printf("%d\n", distance_cible);
	}
}
