#include "geffe.h"

int main(){
  FILE* fichier = NULL;

  fichier = fopen("suite_chiffrante.txt", "w");
  LFSR L0,L1,L2;//les 3 LFSR
  int n = 32; // Taille de la suite chiffrante Si
  _Bool  x0, x1, x2;
  _Bool * tab; //tableau prenant la valeur de retour du filtrage(suite_chiffrante)
  _Bool key_L2[16];

  tab = malloc(sizeof(_Bool)*n);

  // les 3 cléfs (k0,k1,k2)
  _Bool k0[16] = {0, 1, 1, 0, 1, 0, 0,0,1,0,1,1,0,1,1,0},
        k1[16] = {0, 1, 0, 1, 0, 0, 0,0,0,0,0,1,0,1,1,1},
        k2[16] = {0, 1, 0, 1, 0, 0, 1,0,1,0,1,1,0,1,0,0};
  // les 3 coéfficients rétroactifs
  _Bool c0[16] = {0, 0, 0, 0, 0, 0, 0,0,1,0,0,1,0,0,1,1},
        c1[16] = {0, 0, 0, 0, 1, 0, 0,0,1,0,0,0,0,0,1,1},
        c2[16] = {0, 0, 0, 0, 0, 0, 0,0,0,0,1,0,1,1,0,1};
        //ci     15 14 13 12 11 10  9 8 7 6 5 4 3 2 1 0

  _Bool F[8] = {1,0,0,0,1,1,1,0};
  //            0 1 2 3 4 5 6 7

  CLEF K, K_atk;
  K = init_clef(k0,k1,k2);

  //initialisation des LFSR
  L0 = init_LFSR(c0, K.k0);
  L1 = init_LFSR(c1, K.k1);
  L2 = init_LFSR(c2, K.k2);


  printf("Suite Chiffrante :\n");
  int i;
  //fprintf(fichier, "y |x0 1 2 \n", tab[i],x2);
  for (i=0;i<n;i++){
    x0 = calcul_LFSR(&L0);
    x1 = calcul_LFSR(&L1);
    x2 = calcul_LFSR(&L2);
    tab[i] = filtrage(F,x0,x1,x2);
    printf("%d", x0, x1, x2, tab[i]);
    if (fichier != NULL){
        fprintf(fichier, "%d", tab[i]);
      }
  }

  printf("\n");

  // affiche_cas_filtrage();
  // K_atk = attaque(tab,F);
  attaque_L2(c2, tab, F, key_L2);


  //printf("\n");
  //printf("Possible clef de L2:\n");
  for (int laurrianne = 15;laurrianne>=0;--laurrianne){
    printf("%d", key_L2[laurrianne]);
  }
  printf("\n");

  free(tab);//on libère la mémoire
  fclose(fichier);
  return 0;
}
