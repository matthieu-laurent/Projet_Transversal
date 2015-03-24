#include <c8051f020.h>                    // SFR declarations
#include <stdio.h>
#include <intrins.h>
#include <string.h>
//*************************************************************************************************
//*************************************************************************************************
// GESTION DE BUFFER CIRCULAIRE
//
// Code tir� du document Powerpoint: Microcontroller Programming III	MP7-46
//
//*************************************************************************************************

//*************************************************************************************************
// DEFINITION DES MACROS DE GESTION DE BUFFER CIRCULAIRE
//*************************************************************************************************

// Structure de gestion de buffer circulaire
	//rb_start: pointeur sur l'adresse de d�but du buffer 
	// rb_end: pointeur sur l'adresse de fin du buffer	
	// rb_in: pointeur sur la donn�e � lire
	// rb_out: pointeur sur la case � �crire
		
#define RB_CREATE(rb, type) \
   struct { \
     type *rb_start; \	   
     type *rb_end; \	   
     type *rb_in; \
	 type *rb_out; \		
	  } rb

//Initialisation de la structure de pointeurs 
// rb: adresse de la structure
// start: adresse du premier �l�ment du buffer 
// number: nombre d'�l�ments du buffer - 1	(le "-1" n'est � mon avis pas n�cessaire)
#define RB_INIT(rb, start, number) \
         ( (rb)->rb_in = (rb)->rb_out= (rb)->rb_start= start, \
           (rb)->rb_end = &(rb)->rb_start[number] )

//Cette macro rend le buffer circulaire. Quand on atteint la fin, on retourne au d�but
#define RB_SLOT(rb, slot) \
         ( (slot)==(rb)->rb_end? (rb)->rb_start: (slot) )

// Test: Buffer vide? 
#define RB_EMPTY(rb) ( (rb)->rb_in==(rb)->rb_out )

// Test: Buffer plein?
#define RB_FULL(rb)  ( RB_SLOT(rb, (rb)->rb_in+1)==(rb)->rb_out )

// Incrementation du pointeur dur la case � �crire
#define RB_PUSHADVANCE(rb) ( (rb)->rb_in= RB_SLOT((rb), (rb)->rb_in+1) )

// Incr�mentation du pointeur sur la case � lire
#define RB_POPADVANCE(rb)  ( (rb)->rb_out= RB_SLOT((rb), (rb)->rb_out+1) )

// Pointeur pour stocker une valeur dans le buffer
#define RB_PUSHSLOT(rb) ( (rb)->rb_in )

// pointeur pour lire une valeur dans le buffer
#define RB_POPSLOT(rb)  ( (rb)->rb_out )


//*************************************************************************************************

#define       MAX_BUFLEN 32
#define       MY_BUF	 10
static char  xdata outbuf[MAX_BUFLEN];     /* memory for ring buffer #1 (TXD) */
static char  xdata inbuf [MAX_BUFLEN];     /* memory for ring buffer #2 (RXD) */
static  bit   TXactive = 0;             /* transmission status flag (off) */

/* define o/p and i/p ring buffer control structures */
static RB_CREATE(out,unsigned char xdata);            /* static struct { ... } out; */
static RB_CREATE(in, unsigned char xdata);            /* static struct { ... } in; */

//**************************************************************************************************
//**************************************************************************************************
void UART0_ISR(void) interrupt 0x4 {

  if (TI0==1) // On peut envoyer une nouvelle donn�e sur la liaison s�rie
  { 
  	if(!RB_EMPTY(&out)) {
       SBUF1 = *RB_POPSLOT(&out);      /* start transmission of next byte */
       RB_POPADVANCE(&out);             /* remove the sent byte from buffer */
  	}
  	else TXactive = 0;                    /* TX finished, interface inactive */
	TI0 = 0;
  }
  else // RI0 à 1 - Donc une donn�e a �t� re�ue
  {
    if(!RB_FULL(&in)) {
     	*RB_PUSHSLOT(&in) = SBUF1;        /* store new data in the buffer */
		RB_PUSHADVANCE(&in);               /* next write location */
	}
   RI0 = 0;
  }
}
//**************************************************************************************************
void serInit(void) {

  RB_INIT(&out, outbuf, MAX_BUFLEN-1);           /* set up TX ring buffer */
  RB_INIT(&in, inbuf,MAX_BUFLEN-1);             /* set up RX ring buffer */

	// Ajouter ici le code de l'initialisation et de configuration de l'UART0

}
// **************************************************************************************************
// SerOutchar: envoi d'un caract�re dans le buffer de transmission de la liaison s�rie
// *************************************************************************************************
unsigned char serOutchar(char c) {

  if(!RB_FULL(&out))  // si le buffer n'est pas plein, on place l'octet dans le buffer
  {                 
  	*RB_PUSHSLOT(&out) = c;               /* store data in the buffer */
  	RB_PUSHADVANCE(&out);                 /* adjust write position */

  	if(!TXactive) {
		TXactive = 1;                      /* indicate ongoing transmission */
 	    TI0 = 1;//   Placer le bit TI � 1 pour provoquer le d�clenchement de l'interruption
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
unsigned char len,code_err=0;

  for(len = 0; len < strlen(buf); len++)
     code_err +=serOutchar(buf[len]);
  return code_err;
}
// **************************************************************************************************
// MAIN
// **************************************************************************************************
                     /* size of local string buffer */ 
sbit bitin = P1^0;
sbit bitout = P1^1;
static char  userbuf[MY_BUF];

static char  *error = "\n\rError: Buffer overflow.\n\r";

void main(void) {
//long  x, y, z;
unsigned char   len, value, rc = 0;
   WDTCN     = 0xDE;
    WDTCN     = 0xAD;
  serInit();                            
 // EA = 1;                              /* allow interrupts to happen */
while(1) {

	  value =serInchar_tst();
	  if (bitout==1)
	  {
	  value=serOutchar('A');
	  value=serOutchar('B');
	  value=serOutchar('C');

	  }
	  if (bitin==1)
	  {
	     value =serInchar_tst();
	  }
 
  } /* while */
} /* main */
