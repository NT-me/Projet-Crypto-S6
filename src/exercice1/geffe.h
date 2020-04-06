#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>

#define TAILLE 16

/* exemple
L0 : (c15,c14, . . . , c1, c0) = (0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1) -> XOR
NOUS ON TRAVAIL AVEC DES BONS GROS BOOLS
*/

int pow(int a,int b);

typedef struct lfsr{//Structure représentant le LFSR
  //int ID;
  _Bool coef_ret[TAILLE];   //coefficient de rétroaction
  _Bool clef[TAILLE]; // contient la suite chiffrante courante
}LFSR;

typedef struct cle{
  _Bool k0[16];
  _Bool k1[16];
  _Bool k2[16];
}CLEF;

// Permet l'initialisation d'un LFSR avec les valeurs cr et sc, respectivement
// coefficient de rétroaction et suite chiffrante
LFSR init_LFSR(_Bool cr[TAILLE], _Bool sc[TAILLE]);

// Permet d'initialiser la clef K
CLEF init_clef(_Bool i0[16], _Bool i1[16], _Bool i2[16]);


//F juste tableau exemple F[8] = {1,0,0,0,1,1,1,0} (valeurs booléennes)
/*
Fonction de filtrage :à chaque coup d'horloge/itération combine les bits de sortie pour produire un bit de la suite chiffrante défini dans F
*/
_Bool filtrage(_Bool F[8], _Bool x0, _Bool x1, _Bool x2);

// itère pour un lfsr (retourne le x associer au lfsr)
_Bool calcul_LFSR(LFSR * l);

void affiche_cas_filtrage();

//Permet de retrouver la clef d'init de L2 puis le test avec les autres LFSR
int attaque(_Bool cr[16], _Bool sc[32], _Bool F[8], CLEF * res, _Bool c0[16], _Bool c1[16], _Bool c2[16]);

//Retrouve la clé d'initialisation de L0 et de L1.
int attaque_L0_L1(_Bool sc[32], _Bool F[8],CLEF * K, LFSR tmpL0, LFSR tmpL1, LFSR tmpL2);
