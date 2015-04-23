

#include "LIB_BSE_Config_Global.h"
#include "commandes_info_centrale.h"
#include "commandes_info_serializer.h"
#include "Ring_buffer.h"
#include <intrins.h>
#include <stdio.h>
#include <stdlib.h>
#include "Controle_Moteur.h"
#include "Module_M1.h"
#include "servo.h"

#define CMD_OK 1 
#define COEF_NS_US 22 //0.000001/(1/SYSCLK);	// coefficient pour convertir 1 sysclk(env. 45ns) en 1 us



void UART0_ISR(void);
void UART1_ISR(void);

void Gestion_Mouvement(void);
void Gestion_DCT_Obst(void);
void Gestion_Servo(void);
void Gestion_Lumiere(void);
void Reinit_cmd(void);

unsigned int MES_Dist_AV(void);
unsigned int MES_Dist_AR(void);
void delai_us(unsigned long int duree);
