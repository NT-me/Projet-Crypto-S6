#include "geffe.h"

int main(){
  FILE* fichier = NULL;

  fichier = fopen("suite_chiffrante.txt", "w");
  LFSR L0,L1,L2;//les 3 LFSR
  int n = 32, val_ret_atk; // Taille de la suite chiffrante Si
  _Bool  x0, x1, x2;
  _Bool * tab; //tableau prenant la valeur de retour du filtrage(suite_chiffrante)

  tab = malloc(sizeof(_Bool)*n);

  // les 3 cléfs (k0,k1,k2)
  _Bool k0[16] = {0, 1, 1, 0, 1, 0, 0,0,1,0,1,1,0,1,1,0},
        k1[16] = {0, 1, 0, 1, 0, 0, 0,0,0,0,0,1,0,1,1,1},
        k2[16] = {0, 1, 0, 1, 0, 0, 1,0,1,0,0,1,1,1,0,0};
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

  // for(int lp=0; lp<16;lp++){
  //   printf("%d",L0.coef_ret[lp]);
  // }
  // printf("\n");


  // printf("L0: ");
  // for (int iL0=0; iL0<n; ++iL0){
  //   x0 = calcul_LFSR(&L0);
  //   if(iL0%16==0){
  //     printf(" | ");
  //   }
  //   printf("%d", x0);
  //
  // }
  // printf("\n");
  //
  // printf("L1: ");
  // for (int iL1=0; iL1<n; ++iL1){
  //   x1 = calcul_LFSR(&L1);
  //   if(iL1%16==0){
  //     printf(" | ");
  //   }
  //   printf("%d", x1);
  //
  // }
  // printf("\n");
  //
  //
  printf("L2:");
  for (int iL2=0; iL2<n; ++iL2){
    x2 = calcul_LFSR(&L2);
    if(iL2%16==0){
      printf(" | ");
    }
    printf("%d", x2);

  }
  printf("\n");

// Réinit de L2
  L2 = init_LFSR(c2, K.k2);

  printf("SC:");
  int i;
  //fprintf(fichier, "y |x0 1 2 \n", tab[i],x2);
  for (i=0;i<n;i++){
    x0 = calcul_LFSR(&L0);
    x1 = calcul_LFSR(&L1);
    x2 = calcul_LFSR(&L2);
    tab[i] = filtrage(F,x0,x1,x2);
    if(i%16==0){
      printf(" | ");
    }
    printf("%d", tab[i]);
    if (fichier != NULL){
        fprintf(fichier, "%d", tab[i]);
      }
  }
  printf("\n");

  //initialisation des LFSR
  L0 = init_LFSR(c0, K.k0);
  L1 = init_LFSR(c1, K.k1);
  L2 = init_LFSR(c2, K.k2);

  val_ret_atk = attaque(c2, tab, F, &K_atk, L0, L1, L2);
  if (val_ret_atk != -1){
    printf("AT:");
    // --- Affichage k0 ---
    for (int atk0=0; atk0<n; ++atk0){
      if(atk0%16==0){
        printf(" | ");
      }
      printf("%d", K_atk.k0[atk0]);
    }

    // --- Affichage k1 ---
    for (int atk1=0; atk1<n; ++atk1){
      if(atk1%16==0){
        printf(" | ");
      }
      printf("%d", K_atk.k1[atk1]);
    }

    // --- Affichage k2 ---
    for (int atk2=0; atk2<n; ++atk2){
      if(atk2%16==0){
        printf(" | ");
      }
      printf("%d", K_atk.k2[atk2]);
    }
    printf("\nNb itérations: %d\n",val_ret_atk);
  }
  else{
    printf("\nPas de clef trouvée\n");
  }


  free(tab);//on libère la mémoire
  fclose(fichier);
  return 0;
}
