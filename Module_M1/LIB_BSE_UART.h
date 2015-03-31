#ifndef LIB_BSE_UART_H
#define LIB_BSE_UART_H
//------------------------------------------------------------------------------------
// Projet BSE 2014 - Fichier configuration et gestion de l'UART0
//------------------------------------------------------------------------------------
//
// AUTH: FARAMOND Victor & CHENE Charly
// DATE: 06-10-2014
// Target: C8051F02x

void CFG_Clock_UART0();

void CFG_UART0();

char Putchar(char c, char csg_tempo);

char Send_String(char *char_ptr);

char Getchar();

void Demo_UART();

void Analyse_String(char* str);

int calculVitesse(char* p);

int calculAngle(char* p);

int calculAngleServo (char* p);

char* calculCoord(char* p, char c);

char* calculIntensite(char* p);

char* calculDureeAllumage(char* p);

char* calculDureeExtinction(char* p);

void calculNombre(char* p);

#endif
