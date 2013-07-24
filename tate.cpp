#include"tate.h"

void EC_POINT_print(const EC_GROUP *group,EC_POINT * P ,BN_CTX *ctx)
{
	BIGNUM *x = BN_new();
  	BIGNUM *y = BN_new();
  	EC_POINT_get_affine_coordinates_GFp(group,P,x,y,ctx);
  	printf("(\n");
  	BN_print(x,16);
  	printf(",\n");
  	BN_print(y,16);
  	printf(")\n");
}
EC_POINT_CF *EC_POINT_CF_new()
{
    EC_POINT_CF *cf = (EC_POINT_CF*) malloc(sizeof(EC_POINT_CF));
    cf->x = CF_new();
    cf->y = CF_new();
    return cf;
}
int EC_POINT_CF_free(EC_POINT_CF* a)
{
	if(a == NULL)
	{
		return 0;
	}
	
	CF_free(a->x);
	CF_free(a->y);

}
int EC_POINT_CF_set_affine(EC_POINT_CF *pcf,const COMPLEXFILED *x,const COMPLEXFILED *y)
{
    CF_copy(pcf->x,x);
    CF_copy(pcf->y,y);
    return 0;
}

int EC_POINT_CF_get_affine(const EC_POINT_CF *pcf,COMPLEXFILED *x,COMPLEXFILED *y, BN_CTX *ctx)
{
    if(x != NULL)
    {
		CF_copy(x,pcf->x);
    }
    if(y != NULL)
    {
		CF_copy(y,pcf->y);
    }
    return 0;
}

void EC_POINT_CF_print(const EC_POINT_CF *pcf,BN_CTX *ctx)
{

    COMPLEXFILED *x = CF_new();
    COMPLEXFILED *y = CF_new();

    EC_POINT_CF_get_affine(pcf,x,y,ctx);
    CF_print(x);
    CF_print(y);

    CF_free(x);
    CF_free(y);
}


EC_GROUP * EC_GROUP_init(BN_CTX *ctx)
{
	char  p_str[] =  "100000000000000000000000000000000000000070000000000000000000000000000000000000000000006240000000000000000000000000000000000002afb";
	char  a_str[] = "1";
	char  b_str[] = "0";
	char  n_str[] = "10000000000000000000000000000000000000007";
	char  h_str[] = "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000624";

	char  gx_str[] = "71499d18ca692cc4c4cc882d773ce746d95c7ffcc15e1672211421662dc5f96998422afc92e6a3ed54c4b54626093056ae910f01a96e30149422b1fe4d7822e1";
	char  gy_str[] = "6f47d5c35b538d18baf7f00bd44287b9a91a28e82775a36d020aa279a8980355e4cb0a25eac9f5495d39a7bbfbda230e4ea805f69f7266cafa162f001acc2f8b";
	const EC_METHOD *method = EC_GFp_simple_method();
	EC_GROUP  *group  = EC_GROUP_new(method);

	BIGNUM *p = BN_new ();
	BN_hex2bn(&p,p_str);

	BIGNUM *a = BN_new ();
	BN_hex2bn(&a,a_str);

	BIGNUM *b = BN_new ();
	BN_hex2bn(&b,b_str);

	BIGNUM *h = BN_new ();
	BN_hex2bn(&h,h_str);

	BIGNUM *n = BN_new ();
	BN_hex2bn(&n,n_str);

	BIGNUM *gx = BN_new ();
	BN_hex2bn(&gx,gx_str);

	BIGNUM *gy = BN_new ();
	BN_hex2bn(&gy,gy_str);

	EC_GROUP_set_curve_GFp(group,p,a,b,ctx);

	EC_POINT *g = EC_POINT_new(group);

	EC_POINT_set_affine_coordinates_GFp(group,g,gx,gy,ctx);

	EC_GROUP_set_generator(group,g,n,h);


	BN_free(p);
	BN_free(a);
	BN_free(b);
	BN_free(h);
	BN_free(n);
	BN_free(gx);
	BN_free(gy);

	EC_POINT_free(g);

	return group;

}

BIGNUM* EC_POINT_get_slope(const EC_GROUP * group, const EC_POINT * a,const EC_POINT *b, BN_CTX *ctx)
{

	if(!EC_POINT_is_on_curve(group,a,ctx)
		|| !EC_POINT_is_on_curve(group,b,ctx))
	{
		return NULL;
	} 
	else
	{
		BIGNUM * up = BN_new();
		BIGNUM * dw = BN_new();
		BIGNUM * ax = BN_new();
		BIGNUM * ay = BN_new();
		BIGNUM * bx = BN_new();
		BIGNUM * by = BN_new();

		BIGNUM * sp = BN_new();

		BIGNUM * tm = BN_new();
		BIGNUM * tp = BN_new();

		BIGNUM * p  = BN_new();
		BIGNUM * e  = BN_new();
		BIGNUM * f  = BN_new();

		EC_POINT_get_affine_coordinates_GFp(group,a,ax,ay,ctx);
		EC_POINT_get_affine_coordinates_GFp(group,b,bx,by,ctx);
		EC_GROUP_get_curve_GFp(group,p,e,f,ctx);

		if(BN_cmp(ax,bx) != 0)
		{
                        BN_sub(up,ay,by);
			BN_sub(tm,ax,bx);
			BN_mod_inverse(dw,tm,p,ctx);
			BN_mod_mul(sp,up,dw,p,ctx);
                        
		}
		else if(BN_cmp(ay,by) == 0)
		{   
			BIGNUM *v3 = BN_int2bn(3);
			BIGNUM *v2 = BN_int2bn(2);

			BN_mod_sqr(tp,ax,p,ctx);
			BN_mod_mul(tm,v3,tp,p,ctx);


			BN_mod_add(up,tm,e,p,ctx);


			BN_mod_mul(tm,v2,ay,p,ctx);
			BN_mod_inverse(dw,tm,p,ctx);
			BN_mod_mul(sp,up,dw,p,ctx);

			BN_free(v3);
			BN_free(v2);
		}
		else
		{       
			BN_set_posinfinity(sp);

		}

		BN_free(up);
		BN_free(dw);
		BN_free(ax);
		BN_free(ay);
		BN_free(bx);
		BN_free(by);
		BN_free(tm);
		BN_free(tp);
		BN_free(p);
		BN_free(e);
		BN_free(f);
		return sp;

	}


}


COMPLEXFILED * EC_PONIT_gTP(const EC_GROUP * group,const EC_POINT *T,const EC_POINT *P,const EC_POINT_CF *Q,BN_CTX *ctx)
{

    BIGNUM *s =  EC_POINT_get_slope(group,T,P,ctx);
    if(s == NULL)
    {
        return NULL;
    }
    else 
    {
    	COMPLEXFILED * r  = CF_new();
		COMPLEXFILED * Qy = CF_new();
		COMPLEXFILED * Ty = CF_new();
		COMPLEXFILED * Qx = CF_new();
		COMPLEXFILED * Tx = CF_new();
		COMPLEXFILED * Cs = CF_new();

		COMPLEXFILED * tm1 = CF_new();
		COMPLEXFILED * tm2 = CF_new();

		BIGNUM *   v0      = BN_int2bn(0);
		BIGNUM *         x = BN_new();
		BIGNUM *         y = BN_new();
		BIGNUM *         p = BN_new();
       
		EC_POINT_get_affine_coordinates_GFp(group,T,x,y,ctx);
		EC_GROUP_get_curve_GFp(group,p,NULL,NULL,ctx);
		CF_set_values(Ty,p,y,v0,ctx);

		CF_set_values(Tx,p,x,v0,ctx);

		EC_POINT_CF_get_affine(Q,Qx,Qy,ctx);
         
        
		if( !BN_is_posinfinity(s))
		{
 	    	CF_set_values(Cs,p,s,v0,ctx);
	    	CF_sub(tm1,Qx,Tx,ctx);
	    	CF_mul(tm2,tm1,Cs,ctx);
	    	CF_sub(tm1,Qy,Ty,ctx);
	    	CF_sub(r,tm1,tm2,ctx);
		}
		else
		{
	    	CF_sub(r,Qx,Tx,ctx);
		}

		BN_free(v0);
		BN_free(x);
		BN_free(y);
		BN_free(p);
		BN_free(s);

		CF_free(Qy);
		CF_free(Qx);
		CF_free(Ty);
		CF_free(Tx);
		CF_free(Cs);
		CF_free(tm1);
		CF_free(tm2);

		return r;
    }

}

int EC_POINT_Distortion_map(EC_POINT_CF *PF,const EC_GROUP * group,const EC_POINT *P, BN_CTX *ctx)
{

    BIGNUM * p = BN_new(); 
    EC_GROUP_get_curve_GFp(group,p,NULL,NULL,ctx);

    BIGNUM * Px      = BN_new();
    BIGNUM * Py      = BN_new();

    EC_POINT_get_affine_coordinates_GFp(group,P,Px,Py,ctx);

    COMPLEXFILED *x = CF_new();
    COMPLEXFILED *y = CF_new();

    BIGNUM *      xx = BN_new();
    BIGNUM *      xy = BN_new();
    BIGNUM *      yx = BN_new();
    BIGNUM *      yy = BN_new();

    BIGNUM *      v0 = BN_int2bn(0);

    BN_sub(xx,v0,Px);
    BN_zero(xy);

    BN_zero(yx);
    BN_copy(yy,Py);

    CF_set_values(x,p,xx,xy,ctx);
    CF_set_values(y,p,yx,yy,ctx);

    EC_POINT_CF_set_affine(PF,x,y);

    BN_free(p);
    BN_free(Px);
    BN_free(Py);
    BN_free(xx);
    BN_free(xy);
    BN_free(yy);
    BN_free(yx);
    BN_free(v0);

    CF_free(x);
    CF_free(y);

    return 0;
}
COMPLEXFILED * EC_POINT_tate_pair(const EC_GROUP * group,const EC_POINT *P,const EC_POINT *Q, BN_CTX *ctx)
{
    COMPLEXFILED * r  = CF_new(),*f,*tm1;
    BIGNUM * v1 = BN_int2bn(1);
    BIGNUM * v0 = BN_int2bn(0);
    BIGNUM * p  = BN_new(),*order,*num1,*num2;

    EC_POINT_CF *Qpf = EC_POINT_CF_new();

    EC_POINT *T,*tmp;

    int i,bits ;

    EC_GROUP_get_curve_GFp(group,p,NULL,NULL,ctx);

    if(EC_POINT_is_at_infinity(group,P) ||EC_POINT_is_at_infinity(group,Q))
    {
		CF_set_values(r,p,v1,v0,ctx);
		return r;
    }

    EC_POINT_Distortion_map(Qpf,group,Q,ctx);

    T = EC_POINT_new(group);
    tmp = EC_POINT_new(group);
    EC_POINT_copy(T,P);

    f  = CF_new();
    tm1  = CF_new();


    CF_set_values(f,p,v1,v0,ctx);

    order = BN_new();

    EC_GROUP_get_order(group,order,ctx);

    bits = BN_num_bits(order);

    for(i = bits-2 ;i>=0;i--)
    {
        
		COMPLEXFILED * tm2 = EC_PONIT_gTP(group,T,T,Qpf,ctx);
		CF_mul(tm1,f,f,ctx);
		CF_mul(f,tm1,tm2,ctx);

		EC_POINT_add(group,tmp,T,T,ctx);
		EC_POINT_copy(T,tmp);

		CF_free(tm2);

		if( BN_is_bit_set(order,i))
		{
	    	COMPLEXFILED * tm2 = EC_PONIT_gTP(group,T,P,Qpf,ctx);

	    	CF_mul(tm1,f,tm2,ctx);
	    	CF_copy(f,tm1);

	    	EC_POINT_add(group,tmp,T,P,ctx);
	    	EC_POINT_copy(T,tmp);

	    	CF_free(tm2);
		}
    }

    num1 = BN_new();
    num2 = BN_new();

    BN_mul(num1,p,p,ctx);
    BN_sub(num2,num1,v1);
    BN_div(num1,NULL,num2,order,ctx);
    CF_mod_exp(r,f,num1,ctx);

    BN_free(num1);
    BN_free(num2);
    BN_free(order);
    BN_free(v1);
    BN_free(v0);
    BN_free(p);

    CF_free(tm1); 
    CF_free(f);

    EC_POINT_CF_free(Qpf);

    return r;

}
