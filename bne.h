#ifndef HEAD_BNE 
#define HEAD_BNE

#include<openssl/bn.h>
#include"str.h"

int BN_set_posinfinity(BIGNUM *a);

int  BN_is_posinfinity(const BIGNUM *a);

void BN_print(const BIGNUM * a,int radix);

//int2bn
BIGNUM * BN_int2bn(int a);
//str2bn
BIGNUM * BN_str2bn(const char * str,int radix);
//bn2str
char * BN_bn2str(const BIGNUM *a,int radix);

#endif
