#include"bne.h"

int BN_set_posinfinity(BIGNUM *a)
{
	BN_dec2bn(&a,"-1");
    return 0;
}

int  BN_is_posinfinity(const BIGNUM *a)
{
    BIGNUM* v = BN_int2bn(-1);
	if(BN_cmp(v,a) == 0)
    {   
        BN_free(v);
		return 1;
    }
    else
    {   
        BN_free(v);
		return 0;
    }

}

BIGNUM * BN_str2bn(const char * str,int radix)
{
	int i,j,tmp,count;
    char * strt;
    char tv[9];
    BIGNUM *r = BN_new();
    
    switch(radix)
    {
    	case 10:
        	BN_dec2bn(&r,str);
       		break;
	 	case 16:
	   		BN_hex2bn(&r,str);
       		break;
     	case 2:
       		count = strlen(str)%8 ?strlen(str)/8 +1:strlen(str)/8;
       		strt = new char(count);
       		for(i = 0;i < count ;i ++ )
       		{
           		strncpy(tv,str+i*8,8);
           		tv[8] = '\0';
           		tmp = 0;j = 0;
           		while(*(tv+j) != '\0')
           		{
            		tmp = tmp << 1;
             		tmp = tmp| (*(tv+j) - '0');
             		j++;
           		}
           		*(strt+i) = tmp;
       		}
                          
       		BN_bin2bn((const unsigned char *)strt,count,r);
       		break;
	 	default:
	   		BN_free(r);
	   		r = NULL;
	   		break;
	}
	
    return r;

}

BIGNUM * BN_int2bn(int a)
{
    char str[10]; 
    BIGNUM *r = BN_new();
    sprintf(str,"%d",a);
    BN_dec2bn(&r,str);
    return r;
}
   

char * BN_bn2str(const BIGNUM *a,int radix)
{
	char *p = NULL;
	unsigned char s[4096];
	int len;
    switch(radix)
    {
    	case 2:
			len = BN_bn2bin (a, s);		
			p = str2bin(s,len);
			break;
        case 10:
            p = BN_bn2dec (a);
	        break;
        case 16:
	        p = BN_bn2hex(a);
            break;
        default:
            break;

   }
   return p;

}
void BN_print(const BIGNUM * a,int radix)
{
	char *p = BN_bn2str(a,radix);
	
   	if (p)
   	{
    	printf("%s\n", p);
   	}
   	
   	delete p;
}




