#include"cf.h"

COMPLEXFILED *CF_new()
{
	COMPLEXFILED * cf = new COMPLEXFILED;
    cf->p = BN_new();
    cf->r = BN_new();
    cf->i = BN_new();
    return cf;
}

int CF_set_values(COMPLEXFILED *cf ,const BIGNUM *p, const BIGNUM *r, const BIGNUM *i,BN_CTX *ctx)
{
    if(cf == NULL || p == NULL 
       || r == NULL || i == NULL)
    {
        return -1;
    }
    else
    {   
		BIGNUM *result  = BN_new();
        BN_copy(cf->p,p );
		BN_nnmod(result,i,p,ctx);
		BN_copy(cf->i,result);
		BN_nnmod(result,r,p,ctx);
        BN_copy(cf->r,result);
        BN_free(result);
        return 0;
    }
}

int  CF_get_values(const COMPLEXFILED *cf ,BIGNUM *p,BIGNUM *r,BIGNUM *i)
{
    if(p != NULL)
    {
        BN_copy(p,cf->p);
    }
    if(r != NULL)
    {
        BN_copy(r,cf->r);
    }
    if(i != NULL)
    {
        BN_copy(i,cf->i);
    }
    
    return 0;
       
}

int CF_copy(COMPLEXFILED *d,const COMPLEXFILED *s)
{
    BN_copy(d->p,s->p); 
    BN_copy(d->i,s->i); 
    BN_copy(d->r,s->r);

    return 0;
}

int CF_add(COMPLEXFILED *r,COMPLEXFILED *a,COMPLEXFILED *b,BN_CTX *ctx)
{
 
    BIGNUM *p  = BN_new();
    BIGNUM *ar = BN_new();
    BIGNUM *ai = BN_new();
    BIGNUM *br = BN_new();
    BIGNUM *bi = BN_new();
    
 	COMPLEXFILED * a_copy = CF_new();
 	COMPLEXFILED * b_copy = CF_new();
 	
 	CF_copy(a_copy,a);
 	CF_copy(b_copy,b);
 	
    CF_get_values(a_copy,p,ar,ai);
    CF_get_values(b_copy,NULL,br,bi);

    BN_mod_add(r->i,ai,bi,p,ctx);
    BN_mod_add(r->r,ar,br,p,ctx);
    BN_copy(r->p,p);

    BN_free(p);
    BN_free(ar);
    BN_free(ai);
    BN_free(br);
    BN_free(bi);
	CF_free(a_copy);
	CF_free(b_copy);
	
    return 0;

}

int CF_sub(COMPLEXFILED *r,COMPLEXFILED *a,COMPLEXFILED *b,BN_CTX *ctx)
{

    BIGNUM *p  = BN_new();
    BIGNUM *ar = BN_new();
    BIGNUM *ai = BN_new();
    BIGNUM *br = BN_new();
    BIGNUM *bi = BN_new();

	COMPLEXFILED * a_copy = CF_new();
 	COMPLEXFILED * b_copy = CF_new();
 	
 	CF_copy(a_copy,a);
 	CF_copy(b_copy,b);
 	
    CF_get_values(a_copy,p,ar,ai);
    CF_get_values(b_copy,NULL,br,bi);
   
    BN_mod_sub(r->i,ai,bi,p,ctx);
    BN_mod_sub(r->r,ar,br,p,ctx);
    
    BN_copy(r->p,p);

    BN_free(p);
    BN_free(ar);
    BN_free(ai);
    BN_free(br);
    BN_free(bi);
    
	CF_free(a_copy);
	CF_free(b_copy);
    return 0;
}

int CF_mul(COMPLEXFILED *r,COMPLEXFILED *a,COMPLEXFILED *b, BN_CTX *ctx)
{

    BIGNUM *p  = BN_new();
    BIGNUM *ar = BN_new();
    BIGNUM *ai = BN_new();
    BIGNUM *br = BN_new();
    BIGNUM *bi = BN_new();

    BIGNUM *ri = BN_new();
    BIGNUM *ir = BN_new();
    BIGNUM *rr = BN_new();
    BIGNUM *ii = BN_new();

	COMPLEXFILED * a_copy = CF_new();
 	COMPLEXFILED * b_copy = CF_new();
 	
 	CF_copy(a_copy,a);
 	CF_copy(b_copy,b);
    CF_get_values(a_copy,p,ar,ai);
    CF_get_values(b_copy,NULL,br,bi);

    BN_mul(ri,ar,bi,ctx);
    BN_mul(ir,ai,br,ctx);
    BN_mul(rr,ar,br,ctx);
    BN_mul(ii,ai,bi,ctx);

    BN_mod_add(r->i,ri,ir,p,ctx);
    BN_mod_sub(r->r,rr,ii,p,ctx);

    BN_copy(r->p,p);

    BN_free(p);
    BN_free(ar);
    BN_free(ai);
    BN_free(br);
    BN_free(bi);

    BN_free(ri);
    BN_free(ir);
    BN_free(rr);
    BN_free(ii);
	CF_free(a_copy);
	CF_free(b_copy);
	
    return 0;
}

int  CF_div(COMPLEXFILED *r,COMPLEXFILED *a,COMPLEXFILED *b,BN_CTX *ctx)
{
    COMPLEXFILED *n = CF_new();
    COMPLEXFILED *t = CF_new();
    COMPLEXFILED *w = CF_new();
    BIGNUM *x = BN_new();
    BIGNUM *y = BN_new();
    BIGNUM *z = BN_new();

  
    
   	COMPLEXFILED * a_copy = CF_new();
 	COMPLEXFILED * b_copy = CF_new();
 	
 	CF_copy(a_copy,a);
 	CF_copy(b_copy,b);
 	
 	BN_zero(z);
    BN_sub(x,z,b_copy->i);
    CF_set_values(n,b_copy->p,b_copy->r,x,ctx);

    BN_mod_sqr(x,b_copy->r,b_copy->p,ctx);
    BN_mod_sqr(z,b_copy->i,b_copy->p,ctx);
    BN_mod_add(y,x,z,b_copy->p,ctx);
    BN_mod_inverse(z,y,b_copy->p,ctx);
    BN_zero(x);
    CF_set_values(t,b_copy->p,z,x,ctx);
    
    CF_mul(w,n,t,ctx); 
    CF_mul(r,w,a_copy,ctx);
    BN_free(x); 
    BN_free(y);
    BN_free(z);
    
    CF_free(n);
    CF_free(t);
	CF_free(a_copy);
	CF_free(b_copy);
	
    return 0;

}


int CF_mod_exp(COMPLEXFILED *r,COMPLEXFILED *a, const BIGNUM *b,BN_CTX *ctx)
{
    int i ,bits;
   	COMPLEXFILED * a_copy = CF_new();	
 	CF_copy(a_copy,a);
    COMPLEXFILED *tmp = CF_new();

    bits =  BN_num_bits(b);
    CF_copy(r,a_copy);
    CF_copy(tmp,r);

    for(i = bits-2;i >= 0;i--)
    { 
        CF_mul(r,tmp,tmp,ctx);
		CF_copy(tmp,r);

		if( BN_is_bit_set(b,i))
		{
	   		CF_mul(r,tmp,a_copy,ctx);
	    	CF_copy(tmp,r);
		}

    }

    CF_free(tmp);
	CF_free(a_copy);
    return 0;
}

int CF_is_equal(const COMPLEXFILED *a, const COMPLEXFILED *b)
{
    if(BN_cmp(a->p,b->p) 
	|| BN_cmp(a->i,b->i)
	|| BN_cmp(a->r,b->r))
    {
        return 0;   

    }
    else
    {
        return 1;
    }
}

void  CF_print(const  COMPLEXFILED *a)
{
    char *p = BN_bn2hex(a->p);
    char *i = BN_bn2hex(a->i);
    char *r = BN_bn2hex(a->r);

    printf("(\np = %s,\nr = %s,\ni = %s,\n)\n",p,r,i);
}

int  CF_free(COMPLEXFILED *a)
{
	if( a == NULL)
		return 0;
	BN_free(a->i);
	BN_free(a->p);
	BN_free(a->r);

	return 0;

}
