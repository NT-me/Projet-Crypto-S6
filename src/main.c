#include "geffe.h"

int main(){
  LFSR L0,L1,L2;//les 3 LFSR
  int n = 50; // Taille de la suite chiffrante Si
  _Bool  x0, x1, x2;
  _Bool * tab; //tableau prenant la valeur de retour du filtrage(suite_chiffrante)
  tab = malloc(sizeof(_Bool)*n);

  _Bool k0[16] = {0,1,1,0,1,0,0,0,1,0,1,1,0,1,0,0},
        k1[16] = {0,1,0,1,0,0,0,0,0,0,0,1,0,1,1,1},
        k2[16] = {0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0};
//les 3 cléfs (k0,k1,k2)
  _Bool c0[16] = {0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,1},
        c1[16] = {0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,1},
        c2[16] = {0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1};
//les 3 coéfficients rétroactifs
  _Bool F[8] = {1,0,0,0,1,1,1,0};
//              0 1 2 3 4 5 6 7
  L0 = init_LFSR(c0, k0);
  L1 = init_LFSR(c1, k1);
  L2 = init_LFSR(c2, k2);


  int i;

  for (i=0;i<n;i++){
    x0 = calcul_LFSR(&L0);
    x1 = calcul_LFSR(&L1);
    x2 = calcul_LFSR(&L2);
    tab[i] = filtrage(F,x0,x1,x2);
    printf("%d %d %d | %d\n", x0, x1, x2, tab[i]);
  }

  free(tab);//on libère la mémoire
  return 0;
}
