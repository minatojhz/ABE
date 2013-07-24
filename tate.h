#ifndef HEAD_TATE 
#define HEAD_TATE

#include<openssl/bn.h>
#include<openssl/ec.h>
#include"cf.h"
#include"bne.h"

typedef struct{

    COMPLEXFILED *x;
    COMPLEXFILED *y;

}EC_POINT_CF;

void EC_POINT_print(const EC_GROUP *group, EC_POINT *P ,BN_CTX *ctx);

EC_POINT_CF* EC_POINT_CF_new();

int EC_POINT_CF_set_affine(EC_POINT_CF *pcf,const COMPLEXFILED *x,const COMPLEXFILED *y);

int EC_POINT_CF_get_affine(const EC_POINT_CF *pcf,COMPLEXFILED *x,COMPLEXFILED *y, BN_CTX *ctx);

int EC_POINT_CF_free(EC_POINT_CF* a);

//init EC_GROUP
EC_GROUP * EC_GROUP_init(BN_CTX *ctx);

BIGNUM *EC_POINT_get_slope(const EC_GROUP * group, const EC_POINT * a,const EC_POINT *b, BN_CTX *ctx);

COMPLEXFILED * EC_PONIT_gTP(const EC_GROUP * group,const EC_POINT *T,const EC_POINT *P,const EC_POINT_CF *Q,BN_CTX *ctx);

int EC_POINT_Distortion_map(EC_POINT_CF *pf,const EC_GROUP * group,const EC_POINT *p, BN_CTX *ctx);

//tate pair e(P,Q) 其中P是子群中的点,Q为椭圆曲线上任意点,注意顺序
COMPLEXFILED * EC_POINT_tate_pair(const EC_GROUP * group,const EC_POINT *P,const EC_POINT *Q, BN_CTX *ctx);

#endif

