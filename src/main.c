#include "geffe.h"

LFSR init_LFSR(_Bool cr[16], _Bool sc[16]){
  int i;
  LFSR lres;
  for(i=0;i<TAILLE;i++){
    lres.coef_ret[i] = cr[i];
    lres.clef[i] = sc[i];
  }
  return lres;
}

_Bool filtrage(_Bool F[8], _Bool x0, _Bool x1, _Bool x2){
  if ((x0 == 0) && (x1 == 0) && (x2==0)){
    return F[0];
  }
  else if ((x0 == 1) && (x1 == 0) && (x2==0)){
    return F[1];
  }
  else if ((x0 == 0) && (x1 == 1) && (x2==0)){
    return F[2];
  }
  else if ((x0 == 1) && (x1 == 1) && (x2==0)){
    return F[3];
  }
  else if ((x0 == 0) && (x1 == 0) && (x2==1)){
    return F[4];
  }
  else if ((x0 == 1) && (x1 == 0) && (x2==1)){
    return F[5];
  }
  else if ((x0 == 0) && (x1 == 1) && (x2==1)){
    return F[6];
  }
  else if ((x0 == 1) && (x1 == 1) && (x2==1)){
    return F[7];
  }
  else{
    exit(-1);
  }
}

_Bool calcul_LFSR(LFSR * l){//POINTEUR car on change la valeurs des l.clef[i]
  int i;

  _Bool deb_lfsr = 0,res;//ça c'est le bit qui va aller au debut du lfsr
  for(i=0;i<TAILLE;i++){
    if(l->coef_ret[i]==1){
        deb_lfsr = deb_lfsr ^ l->clef[i];
    }
  }
  res = l->clef[15];
  for(i=TAILLE-1;i>0;i--){//TAILLE -1 car on commence à 15 et i>0 car on fini à 1
    l->clef[i] =l->clef[i-1];
  }
  l->clef[0] = deb_lfsr;

  return res;
}



int main(){
  LFSR L0,L1,L2;
  int n = 50000; // Taille de la suite chiffrante Si
  _Bool  x0, x1, x2;
  _Bool * tab;
  tab = malloc(sizeof(_Bool)*n);

  _Bool k0[16] = {0,1,1,0,1,0,0,0,1,0,1,1,0,1,0,0},
        k1[16] = {0,1,0,1,0,0,0,0,0,0,0,1,0,1,1,1},
        k2[16] = {0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0};

  _Bool c0[16] = {0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,1},
        c1[16] = {0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,1},
        c2[16] = {0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1};

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

  free(tab);
  return 0;
}
