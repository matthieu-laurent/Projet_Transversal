                    // SFR declarations
#include <stdio.h>
#include <stdlib.h>
#include <intrins.h>
#include <string.h>
#include "LIB_BSE_Config_Global.h"
#include "Ring_buffer.h"

//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
/*void serInit(void) {

  RB_INIT(&out, outbuf, MAX_BUFLEN-1);           // set up TX ring buffer 
  RB_INIT(&in, inbuf,MAX_BUFLEN-1);             // set up RX ring buffer 

	// Ajouter ici le code de l'initialisation et de configuration de l'UART0

} */

// Déclaration des variables en "extern" indispensable car  ces variables globales provoquent des erreurs via un header (.h), peut-être dû au compilateur de Keil 
extern char  xdata outbuf[MAX_BUFLEN];     // memory for ring buffer #1 (TXD) 
extern char  xdata inbuf [MAX_BUFLEN];     // memory for ring buffer #2 (RXD) 
extern RB_CREATE(out,unsigned char xdata);            // static struct { ... } out; 
extern RB_CREATE(in, unsigned char xdata);            // static struct { ... } in; 
extern bit TXactive;
extern char xdata *msg_info;
// **************************************************************************************************
// SerOutchar: envoi d'un caract�re dans le buffer de transmission de la liaison s�rie
// *************************************************************************************************
unsigned char serOutchar(char c) {

  if(!RB_FULL(&out))  // si le buffer n'est pas plein, on place l'octet dans le buffer
  {                 
  	*RB_PUSHSLOT(&out) = c;               /* store data in the buffer */
  	RB_PUSHADVANCE(&out);                 /* adjust write position */

		 /* On place toujours le char dans le buffer (si pas plein), TXactive déclenche la transmission 
			et repasse à zéro quand la transmission est terminée (dernier char envoyé, c-a-dire, buffer vide) */

  	if(!TXactive) {
		TXactive = 1;                      /* indicate ongoing transmission */
 	    TI0 = 1;//   Placer le bit TI à 1 pour provoquer le d�clenchement de l'interruption
  	}
		else return 0;
	return 1;  // op�ration correctement r�alis�e 
  }
   else return 0; // op�ration �chou�e
}
// ************************************************************************************************
//  serInchar: 	lecture d'un caract�re dans le buffer de r�ception de la liaison s�rie
// ************************************************************************************************
char serInchar(void) {
char c;

  if (!RB_EMPTY(&in))
  {                 /* wait for data */

  	c = *RB_POPSLOT(&in);                 /* get character off the buffer */
 	RB_POPADVANCE(&in);                   /* adjust read position */
  	return c;
  }
  else return 0;
}
// ************************************************************************************************
//  serInchar_tst: 	lecture d'un caract�re dans le buffer de transmission de la liaison s�rie
// ************************************************************************************************
char serInchar_tst(void) {
char c;

  if (!RB_EMPTY(&out))
  {                 /* wait for data */

  	c = *RB_POPSLOT(&out);                 /* get character off the buffer */
 	RB_POPADVANCE(&out);                   /* adjust write position */
  	return c;
  }
  else return 0;
}
// *************************************************************************************************
// serOutstring:  Envoi d'une chaine de caract�re dans le buffer de transmission
// ************************************************************************************************
unsigned char serOutstring(char *buf) {
unsigned char len,lenght,code_err=0;

	lenght = strlen(buf);
  for(len = 0; len < lenght; len++)	// !!!! NE PAS ECRIRE "len < strlen(buf)" CELA PROVOQUE UN DEBORDEMENT DE 'buf' raison inconnue.
     code_err +=serOutchar(buf[len]);
	
  return code_err;
}
unsigned char serOutstring_tst(void) {
unsigned char len,lenght,code_err=0;

	lenght = strlen(msg_info);
  for(len = 0; len < lenght; len++)	// !!!! NE PAS ECRIRE "len < strlen(buf)" CELA PROVOQUE UN DEBORDEMENT DE 'buf' raison inconnue.
     code_err +=serOutchar(msg_info[len]);
	
  return code_err;
}
// **************************************************************************************************
// MAIN
// **************************************************************************************************
                     /* size of local string buffer */ 
sbit bitin = P1^0;
sbit bitout = P1^1;
bit start_test=0;	
static char  userbuf[MY_BUF];

static char  *error = "\n\rError: Buffer overflow.\n\r";

void test_ring_buffer(void) {
//long  x, y, z; 

unsigned char value, rc = 0, id_cmd=0;
unsigned char cmd[30] = "";
	Init_Device();
	
  serInit();                            
 // EA = 1;                              // allow interrupts to happen
	bitin = 0;
	while(1){
	
		if(start_test){
			start_test = 0;
			id_cmd = 0;
		do{
					value = serInchar();
					if(value != 0)
					{
						cmd[id_cmd] = value;
						id_cmd++;
					}
			}while((value != 0) && (value != '\r'));				
				
		if(value == '\r')
		{			
			printf("cmd = %s",cmd);
			// analyse cmd
		}

		if(value == 0)
		{
			//buffer vide : erreur ou ne fait rien
		}			
		
		
		}
	/*while(1) {

	  value =serInchar_tst();
	  if (bitout==1)
	  {
	  value=serOutchar('A');
	  value=serOutchar(' ');
	  value=serOutchar('8');
		value=serOutchar('\r');

	  }
	  if (bitin==1)
	  {
	     value =serInchar_tst();
	  } */
 
  }  /* while */
} /* main */

void delay(int temps){
	int i;
	
	for(i=0;i<temps;i++){}
}
