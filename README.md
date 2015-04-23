# Projet_Transversal

Animateur du projet : Adam Benadada

Responsable planificateur : Damien Perez

Responsable qualité générale : Victor Faramond

Responsable communication : Matthieu Laurent

Responsable respect des consignes : Charly Chene

Responsables qualité test : Leo Schmitt, Stan Renia

Responsable logistique : Pierrick Vinot


# Compte rendu des choses a vérifier/implémenter apres le Jalon n°2

- Apres que le robot ai deja reçu plusieurs commande, il a besoin de prendre en compte 3 commandes pour effectuer une aciton : Verifier l'UART0 et vider les buffers
- Verifier TV (impression qu'une seule roue prend en compte la commande de la vitesse) : le robot avance en diagonale...
- Certaines commandes sont a finir
- Commande pour aller_en() à ajuster (mauvaise compréhension du signe -)
- Regler l'accusé de reception du servo afin qu'il arrive bien lorsque le servo est en position finale et non avant
- Implementation de l'arret d'urgence
- Implementation de l'accusé de réception de la commande Aller_en()


# A faire d'ici la fin du projet

- Finir la liaison SPI
- Faire l'acquisition des signaux sonores
- Faire la commande du pointeur lumineux
