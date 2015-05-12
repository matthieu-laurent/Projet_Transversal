typedef unsigned char BYTE;

enum Epreuve {Epreuve_non, epreuve1, epreuve2, epreuve3, epreuve4, epreuve5, epreuve6, epreuve8, Fin_Epreuve, Stop_Urgence};
enum Mouvement {Mouvement_non, Avancer, Reculer, Stopper, Rot_90D, Rot_90G, Rot_180D, Rot_180G, Rot_AngD, RotAngG, Depl_Coord};
enum ACQ_Son {ACQ_non, ACQ_oui};
enum DCT_Obst {DCT_non, oui_180, oui_360};
enum Lumiere {Lumiere_non, Allumer, Eteindre};
enum Servo {Servo_non, Servo_H, Servo_V, Servo_C};

typedef struct COMMANDES // Cette structure contient toutes les commandes envoyées par la centrale de commande
// Les variables de type enum correspondent aux commandes
// Les variables de type BYTE, UNSIGNED_BYTE ou int correspondent aux paramètres
{
enum Epreuve Etat_Epreuve; //Contient le numéro d'épreuve en cours - Commandes "D" "E" "Q"
enum Mouvement Etat_Mouvement; //Contient le type de mouvement demandé - Commandes "A" "B" "S" "RD" "RG" "RC" "RA" "G"
// Paramètres des commandes précédentes:
BYTE Vitesse; // Paramètre vitesse - Peut aussi être modifié par la commande "TV"
BYTE Coord_X; // Paramètre coordonnée X
BYTE Coord_Y; // Paramètre coordonnée Y
int Angle; // Paramètre angle
enum ACQ_Son Etat_ACQ_Son; // Contient la commande d'acquisition de signaux sonores "ASS"
// Paramètre de la commande précédente:
BYTE ACQ_Duree; // Paramètre durée de la séquence d'acquisition
enum DCT_Obst Etat_DCT_Obst; // Contient la commande de détection d'obstacle "MOB D" "MOB"
// Paramètre de la commandes précédente:
BYTE DCT_Obst_Resolution; // Paramètre résolution angulaire de la détection d'obstacle
enum Lumiere Etat_Lumiere; // Contient la commande d'allumage du pointeur lumineux "L" "LS"
// Paramètres de la commande précédente:
BYTE Lumiere_Intensite; // Paramètre intensité lumineuse
BYTE Lumiere_Duree; // Paramètre durée d'allumage
BYTE Lumiere_Extinction; // Paramètre durée d'extinction
BYTE Lumiere_Nbre; // Paramètre nombre de cycles d'allumage/extinction
enum Servo Etat_Servo; // Contient la commande de positionnement d'un servo "CS C" "CS H" "CS V"
// Paramètre de la commande précédente:
BYTE Servo_Angle; //Paramètre angle de rotation
} OUT_M1;

OUT_M1 cmd_c;
