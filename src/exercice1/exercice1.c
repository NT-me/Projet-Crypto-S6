#include "geffe.h"

int main(){
  LFSR L0,L1,L2;//les 3 LFSR
  int n = 128, val_ret_atk; // Taille de la suite chiffrante Si
  _Bool  x0, x1, x2;
  _Bool * tab; //tableau prenant la valeur de retour du filtrage(suite_chiffrante)

  tab = malloc(sizeof(_Bool)*n);

  // les 3 cléfs (k0,k1,k2)
  _Bool k0[16] = {0, 1, 1, 1, 1, 0, 1,1,1,1,1,1,1,1,1,1},
        k1[16] = {0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,1,0,1,1},
        k2[16] = {0, 0, 1, 0, 0, 0, 0,1,0,0,0,1,0,0,1,0};
  // les 3 coéfficients rétroactifs
  _Bool c0[16] = {0, 0, 0, 0, 0, 0, 0,0,1,0,0,1,0,0,1,1},
        c1[16] = {0, 0, 0, 0, 1, 0, 0,0,1,0,0,0,0,0,1,1},
        c2[16] = {0, 0, 0, 0, 0, 0, 0,0,0,0,0,1,1,1,0,1};
        //ci     15 14 13 12 11 10  9 8 7 6 5 4 3 2 1 0

  _Bool F[8] = {1,0,0,0,1,1,1,0};
  //            0 1 2 3 4 5 6 7

  CLEF K, K_atk;
  K = init_clef(k0,k1,k2);

  //initialisation des LFSR
  L0 = init_LFSR(c0, K.k0);
  L1 = init_LFSR(c1, K.k1);
  L2 = init_LFSR(c2, K.k2);

  for(int rem = 0; rem <16; ++rem){
    L0.clef[rem] = K.k0[rem];
    L1.clef[rem] = K.k1[rem];
    L2.clef[rem] = K.k2[rem];
  }

  printf("K0 : \n");
  for(int coucou=0;coucou<16;++coucou){
    printf("%d", L0.clef[coucou]);
  }
  printf("\n");
  printf("K1 : \n");
  for(int coucou=0;coucou<16;++coucou){
    printf("%d", L1.clef[coucou]);
  }
  printf("\n");
  printf("K2 : \n");
  for(int coucou=0;coucou<16;++coucou){
    printf("%d", L2.clef[coucou]);
  }
  printf("\n");

  printf("\nSuite Chiffrante: ");
  int i;
  for (i=0;i<n;i++){
    x0 = calcul_LFSR(&L0);
    x1 = calcul_LFSR(&L1);
    x2 = calcul_LFSR(&L2);
    tab[i] = filtrage(F,x0,x1,x2);
    printf("%d",tab[i]);
  }
  printf("\n");

  val_ret_atk = attaque(tab, F, &K_atk, c0, c1, c2);
  if (val_ret_atk != -1){
    printf("\n\nAT:");
    // --- Affichage k0 ---
    printf("k0:\n");
    for (int atk0=0; atk0<16; ++atk0){
      printf("%d", K_atk.k0[atk0]);
    }
    printf("\n");


    // --- Affichage k1 ---
    printf("k1:\n");
    for (int atk1=0; atk1<16; ++atk1){
      printf("%d", K_atk.k1[atk1]);
    }
    printf("\n");


    // --- Affichage k2 ---
    printf("k2:\n");
    for (int atk2=0; atk2<16; ++atk2){
      printf("%d", K_atk.k2[atk2]);
    }
    printf("\n");
    printf("\nNb itérations: %d\n",val_ret_atk);
  }
  else{
    printf("\nPas de clef trouvée\n");
  }

  free(tab);//on libère la mémoire
  return 0;
}
