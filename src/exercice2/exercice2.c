#include "feistel.h"

int pow(int a,int b){
	int tmp=1;
	for(int i = 1;i<b+1;i++){
		tmp = a * tmp ;
	}
	return tmp;
}

int bin_to_int(_Bool bin[32]){
  int res=0;
  for(int i=0;i<32;i++){
    res+= bin[i]*pow(2,32-i);
  }
  return res;
}

int main(){

  // Les 2 blocs a crypter
  _Bool testG[32] = {0,1,1,1,0,0,1,0,1,1,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0},
        testD[32] = {1,0,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,1,0,0,1,1,0,1,0,1,1,1,0,0,0,0};

  //Clefs
  _Bool  key1[32] = {0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0},
         key2[32] = {1,0,0,1,1,0,0,0,0,1,1,1,0,1,1,0,0,1,0,1,0,1,0,0,0,0,1,1,0,0,1,0};

  _Bool resG[32], resD[32];

  CLEF K = init_clef(key1,key2);

  tour_feistel(testG, testD,K,resG,resD);

  printf("===== Entrées =====\n");
  // ----------- Affichage des clés après attaque SUR UN TOUR -----------
  CLEF res;

  res = atk_1tour(testD, testG, resD, resG);
  printf("k0 (issue de l'attaque sur 1 tour):\n");
  for(int i = 0;i < 32; i++)
  {
    printf("%d",res.k0[i]);
  }
  printf("\n 0x%x \n",bin_to_int(res.k0));

  printf("k1 (issue de l'attaque sur 1 tour):\n");
  for(int i = 0;i < 32; i++)
  {
    printf("%d",res.k1[i]);
  }
  printf("\n 0x%x \n",bin_to_int(res.k1));

  printf("--- Textes clairs : ---\n\n");
  printf("Gauche : \n");
  for(int jko = 0; jko<32;++jko){
    printf("%d",testG[jko]);
  }
  printf("\n");

  printf("Droite : \n");
  for(int jko = 0; jko<32;++jko){
    printf("%d",testD[jko]);
  }
  printf("\n");
  printf("======================\n");

  // ---------------------------------------------------------------------------

  // ----------- Affichage des clefs issue d'UN TOUR de feistel -----------
printf("\n\n------Affichage d'un tour de Feistel : ---\n");
  printf("Gauche : ");
  for(int i = 0;i < 32; i++)
  {
    printf("%d",resG[i]);
  }
  printf("\n %x \n",bin_to_int(resG));

  printf("Droite : ");
  for(int i = 0;i < 32; i++)
  {
    printf("%d",resD[i]);
  }
  printf("\n %x \n",bin_to_int(resD));
  printf("\n------------------------------------------\n");
  // ---------------------------------------------------------------------------

  //test de feistel pour n tour
  int n = 12;
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
printf("------Affichage de Feistel : %d tours ---\n",n);
  printf("Gauche : ");
  for(int i = 0;i < 32; i++)
  {
    printf("%d",resG[i]);
  }
  printf("\n %x \n",bin_to_int(resG));
  printf("\n");

  printf("Droite : ");
  for(int i = 0;i < 32; i++)
  {
    printf("%d",resD[i]);
  }
  printf("\n %x \n",bin_to_int(resD));

  printf("\n------------------------------------------\n");
  // ---------------------------------------------------------------------------


  return 0;
}
