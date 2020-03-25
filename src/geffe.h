#include "stdio.h"
#include "stdlib.h"

#define TAILLE 16

/* exemple
L0 : (c15,c14, . . . , c1, c0) = (0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1) -> XOR
NOUS ON TRAVAIL AVEC DES BONS GROS BOOLS
*/
typedef struct lfsr{//Structure représentant le LFSR
  //int ID;
  _Bool coef_ret[TAILLE];   //coefficient de retroaction
  _Bool clef[TAILLE]; // contient la suite chiffrante courante
}LFSR;

// Permet l'initialisation d'un LFSR avec les valeurs cr et sc, respectivement
// coefficient de rétroaction et suite chiffrante
LFSR init_LFSR(_Bool cr[TAILLE], _Bool sc[TAILLE]);

//F juste tableau exemple F[8] = {1,0,0,0,1,1,1,0} (valeurs booléennes)
/*
Fonction de filtrage :à chaque coup d'horloge/itération combine les bits de sortie pour produire un bit de la suite chiffrante défini dans F
*/
_Bool filtrage(_Bool F[8], _Bool x0, _Bool x1, _Bool x2);

// itère pour un lfsr (retourne le x associer au lfsr)
_Bool calcul_LFSR(LFSR * l);
