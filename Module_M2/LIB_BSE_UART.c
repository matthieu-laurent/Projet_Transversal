#include <LIB_BSE_UART.h>	
#include <LIB_BSE_Config_Global.h>

void CFG_Clock_UART0(void)
{
	CKCON|=0x10; // timer1 clock select
	TMOD|=0x20; // l'horloge du timer 1 est sysclock
	TMOD&=0x2F; // le timer 1 est configuré en timer 8 bits avec autoreload
	TH1= -(SYSCLK/BAUDRATE/16);
	TL1= 0xff;
	ET1=0; // désactive les interruptions du timer 1
	TR1=1;//Demarrer le Timer1	
	TF1=0;//flag du Timer1 remis à 0
// On démarre le timer 1 à FFh ainsi un coup de clock après avoir démarrer, 
// le timer se précharge avec la valeur de TH1
}

void CFG_UART0(void)
{
	RCLK0=0; // horloges de réception de l'UART0 = Timer 1
	TCLK0=0; // horloges de transmission de l'UART0 = Timer 1
	PCON |=0x80; // SMOD0=1 : UART0 baud rate devided by two disabled
	PCON &= 0xBF; // SSTAT0=0
	SCON0 =0x72; // Mode1 : 8-Bit UART, Variable Baud Rate,
		     //RI0 will only be activated if stop bit is logic level 1,
		    //UART0 reception enabled
	TI0=1;
}

char Putchar(char c, char csg_tempo)
{
	int i;
	while(csg_tempo!=0)
	{
		if(TI0==0){
			for(i=0; i<440;i++){}//temporisation de 20µs
	        csg_tempo--;
			if(csg_tempo==0) return 0;
			}	
		else{
	        SBUF0=c; // Envoi du caractère c dans le registre de données de l'UART
        	TI0=0; // Remise à zéro du drapeau
        	return c;
			}
	}	
	return 0;
	
}

char Send_String(char *char_ptr)
{
	char cpt=0;
	char erreur;
	while(*char_ptr!=0){
        	erreur=Putchar(*char_ptr, 10);
					if(erreur==0) return 0;
        	char_ptr++;
        	cpt++;
	}
	return cpt;
}

char Getchar(void)
{
	char c;
	if(RI0==0) return 0; //Flag=0, pas de caractère reçu
       	c=SBUF0;
       	RI0=0;
       	return c;
}



void Action_UART1()
{
	if(S_INPUTS.Etat_Commande!=Commande_non)
	{
		switch(S_INPUTS.Etat_Commande)
		{
			case mogo_1_2 : 
				Send_String("mogo 1:");
				
				
			
		}
		
	}
	
}

