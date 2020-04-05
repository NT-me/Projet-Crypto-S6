#include "feistel.h"


CLEF init_clef(_Bool  key0[32],_Bool  key1[32]){
  CLEF K;
  for(int zazoo = 0; zazoo < 32; zazoo++){
    K.k0[zazoo] = key0[zazoo];
    K.k1[zazoo] = key1[zazoo];
  }
  return K;
}

void boite7(_Bool mot[32], _Bool * res){
  _Bool tmp[7];
  for (int i = 0; i<7;++i){
    tmp[i] = mot[i];
  }

  for(int i = 0; i<25; ++i){
    res[i] = mot[i+7];
  }

  for(int i = 0; i<7;++i){
    res[i+25] = tmp[i];
  }
}

void xor32(_Bool mot0[32], _Bool mot1[32], _Bool * res)
{
  for(int i = 0; i<32; i++)
  {
    res[i] = mot0[i]^mot1[i];
  }
}

void tour_feistel(_Bool xL[32], _Bool xR[32],CLEF K, _Bool * resG, _Bool * resD)
{
  _Bool tmp[32];
  _Bool tmp1[32];
  _Bool tmp2[32];

  //Premier xor L
  xor32(xL,xR,tmp);
  //Première boite7 L
  boite7(tmp,tmp1);
  //Second XOR L
  xor32(tmp1,K.k0,tmp1);

  //Premier xor R
  xor32(xR,tmp1,tmp);
  //Première boite7 R
  boite7(tmp,tmp2);
  //Second XOR R
  xor32(K.k1,tmp2,resD);

  for(int i = 0;i<32; i++)
    resG[i] = tmp1[i];

}

CLEF atk_1tour(_Bool xR[32], _Bool xL[32], _Bool crypt_xR[32], _Bool crypt_xL[32])
{
  CLEF cle;//la clef contenant k0,k1 (vide pour l'instant)
  _Bool tmp1[32],tmp0[32];

  //K0
  xor32(xR,xL,tmp0);
  boite7(tmp0,tmp1);
  xor32(tmp1,crypt_xL,cle.k0);
  //k1
  xor32(xR,crypt_xL,tmp0);
  boite7(tmp0,tmp1);
  xor32(tmp1,crypt_xR,cle.k1);

  return cle;
}
