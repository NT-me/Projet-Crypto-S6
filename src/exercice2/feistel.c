#include "feistel.h"

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

void tour_feistel(_Bool xL[32], _Bool xR[32], _Bool k0[32],_Bool k1[32], _Bool * resG, _Bool * resD)
{
  _Bool tmp[32];
  _Bool tmp1[32];
  _Bool tmp2[32];

  //Premier xor L
  xor32(xL,xR,tmp);
  //Première boite7 L
  boite7(tmp,tmp1);
  //Second XOR L
  xor32(tmp1,k0,tmp1);

  //Premier xor R
  xor32(xR,tmp1,tmp);
  //Première boite7 R
  boite7(tmp,tmp2);
  //Second XOR R
  xor32(k1,tmp2,resD);

  for(int i = 0;i<32; i++)
    resG[i] = tmp1[i];

}
