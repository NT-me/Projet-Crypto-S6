#include "stdio.h"
#include "stdlib.h"

typedef struct cle{
  _Bool k0[32];
  _Bool k1[32];
}CLEF;

// initilise les clefs
CLEF init_clef(_Bool key0[32],_Bool  key1[32]);
//Shift de 7 vers la gauche
void boite7(_Bool mot[32], _Bool * res);

// Prend deux mots de 32 bits, les XOR et stock le résultat dans res
void xor32(_Bool mot0[32], _Bool mot1[32], _Bool * res);

// Un tour de feistel prenant deux mots de 32 bits en entrée et ressort le mot gauche et le mot droite
void tour_feistel(_Bool xL[32], _Bool xR[32], CLEF K, _Bool * resG, _Bool * resD);

// Fonction d'attaque d'un tour prenant en entrée des textes clairs et des textes cryptés et retourne une struct clef
CLEF atk_1tour(_Bool xR[32], _Bool xL[32], _Bool crypt_xR[32], _Bool crypt_xL[32]);
