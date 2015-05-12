#ifndef c8051f020
#define c8051f020
#include <c8051f020.h>
#endif

void configTimer3(void);
void ISRtimer3(void);


void PCA_Init(void);

void Lumiere (void);
void Lumiere_Stop (void);

char* Envoi_Trame_SPI(void);
int Reception_Trame_SPI(char* str);
void SPI_Init(void);
void it_SPI(void);
