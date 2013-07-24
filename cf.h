#ifndef HEAD_CF
#define HEAD_CF

#include<openssl/bn.h>
#include<stdio.h>

typedef struct {
    BIGNUM *p;
    BIGNUM *r;
    BIGNUM *i;
} COMPLEXFILED;

COMPLEXFILED *CF_new();

int  CF_set_values(COMPLEXFILED *cf ,const BIGNUM *p, const BIGNUM *r, const BIGNUM *i,BN_CTX *ctx);

int  CF_get_values(const COMPLEXFILED *cf ,BIGNUM *p,BIGNUM *r,BIGNUM *i);

int  CF_copy(COMPLEXFILED *d,const COMPLEXFILED *s);


//r = a + b
int  CF_add(COMPLEXFILED *r, COMPLEXFILED *a,COMPLEXFILED *b,BN_CTX *ctx);

//r = a - b 
int  CF_sub(COMPLEXFILED *r,COMPLEXFILED *a,COMPLEXFILED *b,BN_CTX *ctx);

//r = a * b
int  CF_mul(COMPLEXFILED *r,COMPLEXFILED *a,COMPLEXFILED *b,BN_CTX *ctx);

//r = a / b
int  CF_div(COMPLEXFILED *r,COMPLEXFILED *a,COMPLEXFILED *b,BN_CTX *ctx);

//r = a^b
int  CF_mod_exp(COMPLEXFILED *r,COMPLEXFILED *a, const BIGNUM *b, BN_CTX *ctx);

//return a == b
int  CF_is_equal(const COMPLEXFILED *a, const COMPLEXFILED *b);

void CF_print(const COMPLEXFILED *a);

int  CF_free(COMPLEXFILED *a);

#endif
