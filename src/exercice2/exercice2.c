#include "feistel.h"

int main(){

  // Les 2 blocs a crypter
  _Bool testG[32] = {0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,1,0,0,1,0,0},
        testD[32] = {0,1,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,1,0,0,1,1,0,0,1,0,0,0,0,1};

  //Clefs
  _Bool  key1[32] = {0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0},
         key2[32] = {1,0,0,1,1,0,0,0,0,1,1,1,0,1,1,0,0,1,0,1,0,1,0,0,0,0,1,1,0,0,1,0};

  _Bool resG[32], resD[32];


  tour_feistel(testG, testD, key1,key2,resG,resD);

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
  printf("\n");

  return 0;
}
