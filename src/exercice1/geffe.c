#include "geffe.h"

int pow(int a,int b){
	int tmp=1;
	for(int i = 1;i<b+1;i++){
		tmp = a * tmp ;
	}
	return tmp;
}

LFSR init_LFSR(_Bool cr[16], _Bool sdc[16]){
  //initialise un lFSR
  int i;
  LFSR lres;
  for(i=TAILLE-1;i>-1;--i){
    lres.coef_ret[TAILLE-1-i] = cr[i];
    lres.clef[TAILLE-1-i] = sdc[i];
  }
  return lres;
}

CLEF init_clef(_Bool k0[16], _Bool k1[16], _Bool k2[16]){
  CLEF res;
  for (int i=0; i<TAILLE; ++i){
    res.k0[i] = k0[i];
    res.k1[i] = k1[i];
    res.k2[i] = k2[i];
  }

  return res;
}

_Bool filtrage(_Bool F[8], _Bool x0, _Bool x1, _Bool x2){
  //pour chaque triplet (x0,x1,x2) renvoyer le F[i] associé
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

_Bool calcul_LFSR(LFSR * l){//POINTEUR car on change la valeur des l.clef[i]
  int i;

  _Bool deb_lfsr = 0,res; //bit qui va aller au début du lfsr
  for(i=0;i<TAILLE;i++){
    if(l->coef_ret[i]==1){
        deb_lfsr = deb_lfsr ^ l->clef[i];
    }
  }
  res = l->clef[0];
  for(i=0;i<15;i++){//TAILLE -1 car on commence à 15 et i>0 car on finit à 1
    l->clef[i] =l->clef[i+1];
  }
  l->clef[15] = deb_lfsr;

  return res;
}

void affiche_cas_filtrage(){
// affiche sur le terminal tous les cas les plus optimaux possibles
_Bool x0,x1,x2;
_Bool F[8];
  int i,j,tmp = 0;
  for(i=0;i<256;i++){//les 256 cas
    tmp = i;
    for(j=7;j!=-1;j--){ // boucle pour convertir un entier i en binaire
      F[j] = tmp & 0x1 ; //mask avec le dernier bit (0 ou 1), résultat dans F[j]
      tmp = tmp >> 1 ;   //shift pour changer de position
      /*exemple pour i = 56
      0011 1000‬ (en binaire)
      donc F[] = {0,0,1,1,1,0,0,0}
      */
    }
    //compteur de corrélation
    float COR_cmpt_x0 = 0.0, COR_cmpt_x1 = 0.0, COR_cmpt_x2 = 0.0;
    //variable pour le pourcentage de corrélation
    float res_COR0 = 0.0, res_COR1 = 0.0, res_COR2 = 0.0;
    //variable pour stocker le résultat de la fonction de Filtrage
    _Bool filtr;

    for(j=0;j<8;j++){ // boucle pour chaque triplet(x0,x1,x2)
      tmp = j;
      x0= tmp & 0x1 ;
      tmp = tmp >> 1;
      x1= tmp & 0x1 ;
      tmp = tmp >> 1;
      x2= tmp & 0x1 ;
      filtr = filtrage(F,x0,x1,x2);

      if(x0 == filtr){//si corrélation alors on incrémente le compteur
        COR_cmpt_x0++;
      }
      if(x1 == filtr){//idem
        COR_cmpt_x1++;
      }
      if(x2 == filtr){//idem
        COR_cmpt_x2++;
      }

    }
    //moyenne des corrélations (pour avoir le pourcentage)
    res_COR0 = COR_cmpt_x0/8;
    res_COR1 = COR_cmpt_x1/8;
    res_COR2 = COR_cmpt_x2/8;
    if (res_COR0 < 0.5 && res_COR1 < 0.5 && res_COR2 < 0.5){
      //affichage des cas les plus optimaux (et leurs numéros associés)
      printf("Filtrage n°%d | Cor x0 %.2f % | Cor x1 %.2f % | Cor x2 %.2f % \n",i, res_COR0,res_COR1,res_COR2);
    }
  }

}

void attaque_L2(_Bool cr[16], _Bool sc[64], _Bool F[8], _Bool * res){
  _Bool init_L2[16], tmpSi[64];
  LFSR tmpL2;
  int i,j,tmp, flag_Break;
  float nb_similitudes = 0.0;

  for(i=0;i<65536;++i){//les 2^16 cas
    tmp = i;
    flag_Break = 0;
    for(j=15;j>-1;j--){ // boucle pour convertir un entier i en binaire
      init_L2[j] = tmp & 0x1 ; //mask avec le dernier bit (0 ou 1), résul&tat dans tmpL2[j]
      tmp = tmp >> 1 ;   //shift pour changer de position
    }

    tmpL2 = init_LFSR(cr,init_L2);
    for(int lo = 0; lo<64; lo++){
      tmpSi[lo] = calcul_LFSR(&tmpL2);
    }
    nb_similitudes = 0.0;
    for(int glace_au_chocolat = 0; glace_au_chocolat<64; ++glace_au_chocolat){ //Compte les similitudes entre la suite chiffrante originelle
      if(sc[glace_au_chocolat] == tmpSi[glace_au_chocolat]) nb_similitudes ++; //et l'initialisation de L2
      // if(glace_au_chocolat == 24){ // Vérfie si nous en sommes à la 24e itération
      //   if(nb_similitudes < 17){ // Si on est a la 24e itération et qu'il y'a moins de 17 similitudes alors on sors.
      //     flag_Break = 1;
      //     break;
      //   }
      // }
    }

    if((flag_Break == 0) && (((nb_similitudes/64.0)*100.0)>=75.0))
    {
      for(int h = 0;h < 64; h++){
        res[h] = tmpSi[h];
      }
      break;
    }
  }
}

void attaque_L0_L1(_Bool cr[16], _Bool sc[32], _Bool F[8],CLEF K, LFSR tmpL0, LFSR tmpL1, LFSR tmpL2){
  int i,j,k,cmp1 = 0,cmp2=0,tmp,cmpres=0;
  _Bool k0[16],k1[16],k0_sav[16],k1_sav[16];

  _Bool  x0, x1, x2;
  _Bool sortie_chiffrante[32];

  for(i=0;i<16;i++){ // réduction du nombre de cas (environ 25%)
    if(K.k2[i] == sc[i]){
      k0[i] = !sc[i];
      k0_sav[i] = 1;
      k1_sav[i] = 1;
      k1[i] = K.k2[i];
      cmp1++;
      cmp2++;
    }
    else {
      k0_sav[i] = 0;
      k1_sav[i] = 0;
    }
  }

  for(j=0;j<pow(2,16-cmp1);j++){// boucle cas k1
    tmp = j;
    for(i=15;i>-1;i--){// création k1
      if(k1_sav[i] == 0){//si la case i n'est pas initialisé
        k1[i] = tmp & 0x1 ;
        tmp = tmp >> 1 ;
      }
    }
    //lfsr 1 et 2
    for(i=0;i<16;i++){ //reduction des cas pour x0
      if (k1[i]==K.k2[i]){
        k0[i]=sc[i];
        k0_sav[i] = 1;
      }
    }

    for(i=0;i<pow(2,16-cmp2);i++){ //pour chaque
      tmp = i;
      for(k=15;k>-1;k--){ // création de k0
        if(k0_sav[i] == 0){
          k0[k] = tmp & 0x1 ;
          tmp = tmp >> 1 ;
        } // Extraire Sortie chiffrante et comparaison avec suite chiffrante
      }
      //init des LFSR avec les cléfs temporaire
      tmpL0 = init_LFSR(tmpL0.coef_ret, k0);
      tmpL1 = init_LFSR(tmpL1.coef_ret, k1);
      tmpL2 = init_LFSR(tmpL2.coef_ret, K.k2);
      cmpres=0;
      for(k=0;k<32;k++){
        x0 = calcul_LFSR(&tmpL0);
        x1 = calcul_LFSR(&tmpL1);
        x2 = calcul_LFSR(&tmpL2);
        sortie_chiffrante[i]=filtrage(F,x0,x1,x2);
        if(sortie_chiffrante[i] == sc[i])
        cmpres++;
      }
      if(cmpres == 32){
        for(k=0;k<16;k++){
          K.k0[k] = k0[k];
          K.k1[k] = k1[k];

        }
      }
    }
  }
}

CLEF attaque(_Bool cr[16], _Bool sc[32], _Bool F[8]){
  CLEF res;

  //attaque_L2(cr,sc,F,&res.k2); //Recherche de la clé d'initialisation de L2

  return res;
}
