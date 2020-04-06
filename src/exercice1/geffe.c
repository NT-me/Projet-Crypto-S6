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

int attaque(_Bool cr[16], _Bool sc[128], _Bool F[8], CLEF * res, _Bool c0[16], _Bool c1[16], _Bool c2[16]){
  _Bool init_L2[16], tmpSi[128];
	_Bool K_vide [16] = {0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0};
  LFSR tmpL2;
  int i,j,tmp, flag_Break,test;
  float nb_similitudes = 0.0;

	LFSR L0 = init_LFSR(c0, K_vide);
	LFSR L1 = init_LFSR(c1, K_vide);
	LFSR L2 = init_LFSR(c2, K_vide);

  for(i=0;i<65536;++i){//les 2^16 cas
    tmp = i;
    flag_Break = 0;
    for(j=15;j>-1;j--){ // boucle pour convertir un entier i en binaire
      init_L2[j] = tmp & 0x1 ; //mask avec le dernier bit (0 ou 1), résul&tat dans tmpL2[j]
      tmp = tmp >> 1 ;   //shift pour changer de position
    }

    tmpL2 = init_LFSR(cr,init_L2);
		for(int rp=0; rp<16;++rp){
			tmpL2.clef[rp] = init_L2[rp];
		}

    for(int lo = 0; lo<128; lo++){
      tmpSi[lo] = calcul_LFSR(&tmpL2);
    }

    nb_similitudes = 0.0;
    for(int glace_au_chocolat = 0; glace_au_chocolat<128; ++glace_au_chocolat){ //Compte les similitudes entre la suite chiffrante originelle
      if(sc[glace_au_chocolat] == tmpSi[glace_au_chocolat]) nb_similitudes ++; //et l'initialisation de L2
      // if(glace_au_chocolat == 24){ // Vérfie si nous en sommes à la 24e itération
      //   if(nb_similitudes < 17){ // Si on est a la 24e itération et qu'il y'a moins de 17 similitudes alors on sors.
      //     flag_Break = 1;
      //     break;
      //   }
      // }
    }

    if((flag_Break == 0) && (((nb_similitudes/128.0)*100.0)>=50.0))
    {
      for(int h = 0;h < 16; h++){//Copie pour passer la clé dans la fonction d'attaque de L0 et L1
        res->k2[h] = tmpSi[h];
      }
			//float yolo = (nb_similitudes/128.0)*100.0;
			//printf("similitude : %f \n", yolo);


			test = attaque_L0_L1(sc,F,res,L0,L1,tmpL2);
			//printf("%d\n",test);

			if(test == 1) return i; // Retourne le nombre d'itération si ça marche
    }
  }
	return -1; // cas d'erreur
}

int attaque_L0_L1( _Bool sc[128], _Bool F[8],CLEF * K, LFSR tmpL0, LFSR tmpL1, LFSR tmpL2){
  int i,j,k,cmp0=0,cmp1,cmp2=0,tmp,cmpres=0, flag = 1,comp_x1;
  _Bool k0[16],k1[16],k0_sav[16],k1_sav[16];
	LFSR tmpL0_,tmpL1_,tmpL2_,compL1;

  _Bool  x0, x1, x2;
  _Bool sortie_chiffrante[128];

	// for(int jio=0;jio<16;++jio){
	// 	printf("%d",tmpL2.clef[jio]);
	// }

 // A revoir cause des erreurs
  for(i=0;i<16;i++){ // réduction du nombre de cas (environ 25%)
    if(K->k2[i] != sc[i]){
      k0[i] = !sc[i];
			k1[i] = !sc[i];
      k0_sav[i] = 1;
      k1_sav[i] = 1;
      cmp0++;
      cmp2++;
    }
    else {
      k0_sav[i] = 0;
      k1_sav[i] = 0;
    }
  }

  for(j=0;j<pow(2,16);j++){// boucle cas k1
    tmp = j;
    for(i=15;i>-1;i--){// création k1
      k1[i] = tmp & 0x1 ;
      tmp = tmp >> 1 ;
    }

		//init le LFSR l1
		compL1=init_LFSR(tmpL0.coef_ret,tmpL0.clef);
		for(int anti_inverse = 0; anti_inverse<16; ++anti_inverse){
			compL1.clef[anti_inverse] = k1[anti_inverse];
		}

		//tour de lFSR l1
		cmp1 = 0;
		for(int kek = 0; kek < 128; kek++){
			comp_x1 = calcul_LFSR(&compL1);

			//comparaison k1 sc
			if(comp_x1 == sc[i]){
				cmp1++;
			}
		}

		if(cmp1>=32){ // Permet de ne pas continuer si la suite chiffrante obtenue grâce à k1 n'est pas égale à au moins 25% de sc
		//printf("%d\n",j);
			//lfsr 1 et 2
			for(i=0;i<16;i++){ //reduction (50%) des cas pour x0
				if (k1[i]==K->k2[i]){
					k0[i]=!sc[i];
					k0_sav[i] = 1;
				}
			}
// --------------------- K0 ---------------------
			for(i=0;i<pow(2,16-cmp2);i++){ //pour chaque k0
				flag = 1;
				tmp = i;
				for(k=15;k>-1;k--){ // création de k0
					if(k0_sav[k] == 0){
						k0[k] = tmp & 0x1 ;
						//printf("%d", k0[k]);
						tmp = tmp >> 1 ;
						} // Extraire Sortie chiffrante et comparaison avec suite chiffrante
					}

					for(int anti_inverse = 0; anti_inverse<16; ++anti_inverse){
						tmpL0.clef[anti_inverse] = k0[anti_inverse];
						tmpL1.clef[anti_inverse] = k1[anti_inverse];
						tmpL2.clef[anti_inverse] = K->k2[anti_inverse];
					}

					for(k=0;k<128;k++){
						x0 = calcul_LFSR(&tmpL0);
						x1 = calcul_LFSR(&tmpL1);
						x2 = calcul_LFSR(&tmpL2);
						sortie_chiffrante[k]=filtrage(F,x0,x1,x2);
						//printf("%d",sortie_chiffrante[k]);
						if(sortie_chiffrante[k] != sc[k]){
							flag = 0;
							break; // Si c'est pas la même on lâche et on continue
						}
					}
					//printf("\n");
					if (flag){
						for(k=0;k<16;k++){
							K->k0[k] = k0[k];
							K->k1[k] = k1[k];
						}
						return 1;
					}
				}
			}
		}
 return -1;
}
