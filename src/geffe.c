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

void affiche_cas_filtrage(){
_Bool x0,x1,x2;
_Bool F[8];

  /*Boucle
  56 cmp_posi = .0
  0011 1000 = i ;
  mask check le dernier
  si dernier a 1 alors a la posi on met un 1
  sinon 0
  shift cmp_posi++;
  */
  int i,j,k,tmp = 0;
  for(i=0;i<256;i++){
    tmp = i;
    for(j=0;j<8;j++){
      F[j] = tmp & 0x1 ; //mask avec le dernier bit (0 ou 1)
      tmp = tmp >> 1 ;//shift pour changer de position
      //printf("F[%d] : %d \n",j,F[j]);
    }

    float COR_cmpt_x0 = 0.0, COR_cmpt_x1 = 0.0, COR_cmpt_x2 = 0.0;
    float res_COR0 = 0.0, res_COR1 = 0.0, res_COR2 = 0.0;
    _Bool filtr;

    for(j=0;j<8;j++){
      tmp = j;
      x0= tmp & 0x1 ;
      tmp = tmp >> 1;
      x1= tmp & 0x1 ;
      tmp = tmp >> 1;
      x2= tmp & 0x1 ;
      filtr = filtrage(F,x0,x1,x2);

      if(x0 == filtr){
        COR_cmpt_x0++;
      }
      if(x1 == filtr){
        COR_cmpt_x1++;
      }
      if(x2 == filtr){
        COR_cmpt_x2++;
      }

      //printf("%d %d %d | %d \n",x0,x1,x2,filtr);
      //BIG IF qui check si x0.x1.x2 == sortie (filtrage())
    }
    res_COR0 = COR_cmpt_x0/8;
    res_COR1 = COR_cmpt_x1/8;
    res_COR2 = COR_cmpt_x2/8;
    // if (res_COR0 < 0.5 && res_COR1 < 0.5 && res_COR2 < 0.5){
    //   printf("Filtrage n°%d % | Cor x0 %.2f % | Cor x1 %.2f % | Cor x2 %.2f % \n\n",i, res_COR0,res_COR1,res_COR2);
    // }
    printf("Filtrage n°%d % | Cor x0 %.2f % | Cor x1 %.2f % | Cor x2 %.2f % \n\n",i, res_COR0,res_COR1,res_COR2);

    //affiche le porcentage de corrélation de chaque xi
  }
  //affiche resultats stats
}
