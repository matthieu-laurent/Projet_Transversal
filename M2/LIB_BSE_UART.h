

void CFG_Clock_UART1(void);
void CFG_UART1(void);
char Putchar(char c, char csg_tempo);
char Send_String(char *char_ptr);
char Getchar(void);
void Action_UART1();
void Init_Commande_Serializer();
void Avancer(int vitesse);
void Reculer(int vitesse);
void Arret();
void Rotation_Droite_90(int vitesse);
void Rotation_Gauche_90(int vitesse);
void Rotation_180(int vitesse);
void Rotation_angle(int angle,int vitesse);