#include "Polynomial.h"

Polynomial::Polynomial()
{
	p = BN_new();
    coefficients = new vector<BIGNUM *>();
}
void Polynomial::SetCoefficients(vector<BIGNUM *> *coefficients)
{
	this->coefficients->clear();
	vector<BIGNUM *>::iterator iter;  
    for (iter=coefficients->begin();iter!= coefficients->end();iter++)  
	{
		BIGNUM * bigNum = BN_new();
		BN_copy(bigNum,*iter);		
		this->coefficients->push_back(bigNum);
	}

}
Polynomial::Polynomial(const Polynomial &PL)
{
	this->p = BN_new();
	this->coefficients =  new vector<BIGNUM *>();
	BN_copy(this->p,PL.p);
	vector<BIGNUM *>::iterator iter;  
    for (iter=PL.coefficients->begin();iter!=PL.coefficients->end();iter++)  
	{
		BIGNUM * bigNum = BN_new();
		BN_copy(bigNum,*iter);		
		this->coefficients->push_back(bigNum);
	}
	
}
Polynomial::Polynomial(vector<BIGNUM *> *coefficients,BIGNUM *p)
{
	this->p = BN_new();
	this->coefficients =  new vector<BIGNUM *>();
	BN_copy(this->p,p);
	vector<BIGNUM *>::iterator iter;  
    for (iter=coefficients->begin();iter!=coefficients->end();iter++)  
	{
		BIGNUM * bigNum = BN_new();
		BN_copy(bigNum,*iter);		
		this->coefficients->push_back(bigNum);
	}

}
void Polynomial::SetP(BIGNUM *p)
{
	BN_copy(this->p,p);
}
Polynomial::Polynomial(BIGNUM *constant,BIGNUM *p,int len)
{
	this->p = BN_new();
    coefficients = new vector<BIGNUM *>();
    BIGNUM *conCopy = BN_new();
    
    BN_copy(this->p,p);
    BN_copy(conCopy,constant);
    
    coefficients->push_back(conCopy);
    
    for(int i=1;i<len;i++)
    {
    	BIGNUM * rn = BN_new();
       	BN_pseudo_rand_range(rn,p);
       	coefficients->push_back(rn);
    }
    
    while(BN_is_zero(*(coefficients->end()-1)))
    {
    	coefficients->pop_back();
    	BIGNUM * rn = BN_new();
       	BN_pseudo_rand_range(rn,p);
       	coefficients->push_back(rn);
    }

}
BIGNUM * Polynomial::GetFunctionValue(BIGNUM *x)
{
    BN_CTX *ctx = BN_CTX_new();

    BIGNUM * value = BN_new() ;

    BN_copy(value,*(coefficients->begin())); 

    BIGNUM * tmp  = BN_new();
    BIGNUM * tmpx = BN_new();
    BN_one(tmpx);
    vector<BIGNUM *>::iterator iter; 
    for (iter=coefficients->begin()+1;iter!= coefficients->end();iter++)  
    {
         BN_mod_mul(tmpx,tmpx,x,p,ctx);
         BN_mod_mul(tmp,tmpx,*iter,p,ctx);	
         BN_mod_add(value,tmp,value,p,ctx);
    }
    BN_free(tmp);
    BN_free(tmpx);
    BN_CTX_free(ctx);
    return value;
}  

BIGNUM * Polynomial::GetLagIntCoe(vector<int> *allParties,int index)
{
   	BN_CTX *ctx = BN_CTX_new();
   	BIGNUM *up = BN_new();
   	BN_one(up);
   	BIGNUM *down = BN_new();
   	BN_one(down);
   	vector<int>::iterator iter;  
   	for (iter=allParties->begin();iter!= allParties->end();iter++)  
   	{
    	if(*iter == index)
        	continue;    
     	BIGNUM *j   = BN_int2bn(-(*iter));
     	BIGNUM *i_j = BN_int2bn(index-(*iter));
    	BN_mul(up,up,j,ctx);
     	BN_mul(down,down,i_j,ctx);
   	}

   	BIGNUM * result = BN_new();
   	BN_mod_inverse(result,down,p,ctx);
   	BN_mod_mul(result,result,up,p,ctx);
   
   	BN_free(up);
  	BN_free(down);
   	BN_CTX_free(ctx);

   	return result;
}

Polynomial::~Polynomial()
{
	BN_free(p);
	vector<BIGNUM*>::iterator iter;
	for(iter=coefficients->begin();iter!=coefficients->end();iter++)
	{
		BN_free(*iter);
	}
}

Polynomial & Polynomial::operator= (const Polynomial &PL)
{
	BN_copy(this->p,PL.p);
	this->coefficients->clear();
	vector<BIGNUM *>::iterator iter;  
    for (iter=PL.coefficients->begin();iter!=PL.coefficients->end();iter++)  
	{
		BIGNUM * bigNum = BN_new();
		BN_copy(bigNum,*iter);		
		this->coefficients->push_back(bigNum);
	}

}
ostream& operator<<(ostream &os,Polynomial &PL)
{
	char *num = NULL;
	int count = 0;  
	bool flag = false;
	vector<BIGNUM *>::iterator iter;
    for (iter=PL.coefficients->begin();iter!=PL.coefficients->end();iter++)  
	{
		num= BN_bn2dec (*iter); 
		if(flag)
		{
			if(strcmp(num,"0"))
			{
				os<<" + "<<endl;
				os<<num<<"."<<"x^"<<count;
			}
		}
		else
		{
			if(strcmp(num,"0"))
			{
				flag = true;
				os<<num;
				if(count)
					os<<"x^"<<count;
			}
		
		}	
		count++;	
	}
	if(!flag)
	{
		os<<"0";
	}
	os<<endl<<"% "<<BN_bn2dec(PL.p)<<endl;
	return os;
}
