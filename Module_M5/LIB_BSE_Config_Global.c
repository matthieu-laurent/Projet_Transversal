#include <LIB_BSE_Config_Global.h>
#include <LIB_BSE_GPIO.h>
#include <LIB_BSE_GPIO.h>
#include <stdlib.h>

unsigned int xdata mempool_global[200];
char xdata mempool_msginfo[3000];

void Init_Device(void)
{
	init_mempool(&mempool_global,sizeof(mempool_global));
	init_mempool(&mempool_msginfo,sizeof(mempool_msginfo));
	gestion_watchdog();
	gestion_horloge();
	
	gestion_interruption();
	gestion_brocheIO();
	CFG_UART0();
	CFG_Timer0();
	CFG_Clock_UART0();
	CFG_UART1();
	CFG_Clock_UART1();
	PCA_Init();
	//	gestion_reset();
	//gestion_memoires();
	//gestion_puissance();
}

void gestion_reset(void)
{
}

void gestion_watchdog(void)
{
// Utilisation du registre WDTCN
	EA=0;//désactivation des interruptions car les deux lignes suivantes doivent être exécutées à la suite.
	WDTCN=0xDE;// désactivation du watchdog
	WDTCN=0xAD;// désactivation du watchdog
	EA=1;//activation des interruptions
}

void gestion_horloge(void)
{
//utilisation des registres OSCXCN et OSCICN
	int i = 0;
	OSCXCN    = 0x67;// crystal oscillator mode + f> 6, 7MHz + osc. not yet stable
	for (i = 0; i < 3000; i++);  // Wait 1ms for initialization
	while ((OSCXCN & 0x80) == 0); // on attend que l'oscillateur soit stable
	OSCICN    = 0x0C; // utilisation de l'oscillateur externe
}


void gestion_brocheIO(void)
{
	Config_GPIO();
}

void gestion_interruption(void)
{
	EX0=0; // Autortise l’interrupion de INT0
}

void CFG_UART1(void)
{
	PCON |=0x10; // UART1 baud rate devided by two disabled
	SCON1 =0x50; // Mode1 : 8-Bit UART, Variable Baud Rate,
		     //RI1 will only be activated if stop bit is logic level 1,
		    //UART1 reception enabled	
	EIE2|=0x40;
}
void CFG_Clock_UART1(void)
{ // UART1 utilise Timer4
	CKCON|=0x40;
  RCAP4L    = 0xDC;
  RCAP4H    = 0xFF;
	T4CON     = 0x34;
}

void CFG_Timer0(void){
	// Init Timer 0
	TMOD=0x01; // timer0 en mode 1 (16bit Counter/timer)
	CKCON|=0x08; // system clock
	// IT0 = 1; // ECHO_ultrason is edge trig_ultrasongered
	ET0 = 1;	// Active Interruption Timer0
}
