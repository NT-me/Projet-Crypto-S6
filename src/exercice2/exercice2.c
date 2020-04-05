#include "feistel.h"

int main(){

  // Les 2 blocs a crypter
  _Bool testG[32] = {0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,1,0,0,1,0,0},
        testD[32] = {0,1,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,1,0,0,1,1,0,0,1,0,0,0,0,1};

  //Clefs
  _Bool  key1[32] = {0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0},
         key2[32] = {1,0,0,1,1,0,0,0,0,1,1,1,0,1,1,0,0,1,0,1,0,1,0,0,0,0,1,1,0,0,1,0};

  _Bool resG[32], resD[32];

  CLEF K = init_clef(key1,key2);

  tour_feistel(testG, testD,K,resG,resD);

  // ----------- Affichage des clefs issue d'UN TOUR de feistel -----------
printf("------Affichage d'un tour de Feistel : ---\n");
  printf("Gauche : ");
  for(int i = 0;i < 32; i++)
  {
    printf("%d",resG[i]);
  }
  printf("\n");

  printf("Droite : ");
  for(int i = 0;i < 32; i++)
  {
    printf("%d",resD[i]);
  }
  printf("\n------------------------------------------\n");
  // ---------------------------------------------------------------------------

  // ----------- Affichage des clés après attaque SUR UN TOUR -----------
  CLEF res;

  res = atk_1tour(testD, testG, resD, resG);
  printf("k0 : ");
  for(int i = 0;i < 32; i++)
  {
    printf("%d",res.k0[i]);
  }
  printf("\n");

  printf("k1 : ");
  for(int i = 0;i < 32; i++)
  {
    printf("%d",res.k1[i]);
  }
  printf("\n");
  // ---------------------------------------------------------------------------

  //test de feistel pour n tour
  int n =12*4*12345;
  _Bool tmpG[32],tmpD[32];
  for(int o=0;o<32;o++){
    tmpG[o] = testG[o];
    tmpD[o] = testD[o];
  }

  for(int banana = 0; banana < n ; banana++){
    tour_feistel(tmpG,tmpD,K, resG,resD);
    for(int e=0; e<32;++e){
      tmpG[e] = resG[e];
      tmpD[e] = resD[e];
    }
  }

  // ----------- Affichage des clefs issue de n TOURS de feistel -----------
printf("------Affichage d'un tour de Feistel : %d tours ---\n",n);
  printf("Gauche : ");
  for(int i = 0;i < 32; i++)
  {
    printf("%d",resG[i]);
  }
  printf("\n");

  printf("Droite : ");
  for(int i = 0;i < 32; i++)
  {
    printf("%d",resD[i]);
  }
  printf("\n------------------------------------------\n");
  // ---------------------------------------------------------------------------


  return 0;
}
