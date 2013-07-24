#include "ABC.h"
void  ABC::InitGroup(EC_GROUP *group)
{
	const EC_METHOD *method = EC_GFp_simple_method();
	this->group = EC_GROUP_new(method);
	EC_GROUP_copy(this->group,group);
}
void ABC::Init(BIGNUM *a0,BIGNUM *a1,EC_GROUP *group)
{
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM   *order = BN_new();
    EC_GROUP_get_order(group,order,ctx);
	const EC_POINT * g = EC_GROUP_get0_generator(group);
    COMPLEXFILED * gg = EC_POINT_tate_pair(group,g,g,ctx);

    COMPLEXFILED * e  = CF_new();
    CF_mod_exp(e,gg,a0,ctx);

    InitGroup(group); 

    EC_POINT *h = EC_POINT_new(group);  
    EC_POINT_mul(group,h,NULL,g,a1,ctx);

    EC_POINT *f = EC_POINT_new(group);
    BIGNUM *a1Inverse = BN_new();
    BN_mod_inverse(a1Inverse,a1,order,ctx);
    EC_POINT_mul(group,f,NULL,g,a1Inverse,ctx);

    pk = ABCPK(group,h,f,e);

    EC_POINT *b = EC_POINT_new(group);
    EC_POINT_mul(group,b,NULL,g,a0,ctx);

    mk = ABCMK(group,a1,b);

    BN_free(order);
    BN_free(a1Inverse);
    CF_free(gg);
    CF_free(e);
    EC_POINT_free(h);
    EC_POINT_free(f);
    EC_POINT_free(b);
    BN_CTX_free(ctx);

}
ABC::ABC(BIGNUM *a0,BIGNUM *a1,EC_GROUP *group)
{
	Init(a0,a1,group);
}

ABC::ABC(EC_GROUP *group)
{
	BN_CTX *ctx = BN_CTX_new();
	BIGNUM   *order = BN_new();
	EC_GROUP_get_order(group,order,ctx); 

	BIGNUM   *a0 = BN_new();
	BIGNUM   *a1 = BN_new();
	BN_pseudo_rand_range(a0,order);
	BN_pseudo_rand_range(a1,order);

	Init(a0,a1,group); 

	BN_free(a0);
	BN_free(a1);
	BN_CTX_free(ctx);
}

ABC::ABC()
{
	const EC_METHOD *method = EC_GFp_simple_method();
	this->group = EC_GROUP_new(method);
}
EC_POINT* ABC::String2Point(char*str)
{
	if(str == NULL)
   	{
		return NULL;
   	}
   	
   	char hashv[50];
   	
   	BN_CTX *ctx = BN_CTX_new();
   	EC_POINT *P = EC_POINT_new(group);
   	const EC_POINT * g = EC_GROUP_get0_generator(group);

   	hash(str,hashv);
   	BIGNUM *b = BN_str2bn(hashv,16);
   	EC_POINT_mul(group,P,NULL,g,b,ctx);
    
   	BN_free(b);
   	BN_CTX_free(ctx);  
  
   	return P;
   
}

BIGNUM * ABC::Decrypt(ABCEBLOCK &eblock)
{
	BN_CTX *ctx = BN_CTX_new();
  	BIGNUM *p = BN_new();
  	BIGNUM *order = BN_new();
  	EC_GROUP_get_order(group,order,ctx);
  	EC_GROUP_get_curve_GFp(group, p,NULL,NULL, ctx);
  
	BIGNUM * r = BN_new();
	COMPLEXFILED * value = CF_new();
	COMPLEXFILED * tmp = CF_new();

	CF_set_values(value,p,BN_int2bn(1),BN_int2bn(0),ctx);

	AccessTree at = AccessTree(eblock.GetPolicy());
	vector<string> *atrs = sk.GetSKAtrNames();
	
	
	vector<ATRPATHLCM> *lcmVec = at.GetATRPATHLCM(atrs,order);

	vector<ATRPATHLCM>::iterator iter;
		  
	for (iter=lcmVec->begin();iter!=lcmVec->end();iter++)
	{
		string name  = iter->GetName();
		BIGNUM *lcm = iter->GetLCM();

		ATRGROUP gc = eblock.GetAGByName(name);
		ATRGROUP gd = sk.GetAGByName(name);  
		COMPLEXFILED * u = EC_POINT_tate_pair(group,gc.GetC1(),gd.GetC1(),ctx);
		COMPLEXFILED * d = EC_POINT_tate_pair(group,gc.GetC2(),gd.GetC2(),ctx);

		CF_div(tmp,u,d,ctx);
		CF_mod_exp(tmp,tmp,lcm,ctx);
		CF_mul(value,value,tmp,ctx);
		BN_free(lcm);
		CF_free(u);
		CF_free(d);
	}
	
	CF_free(tmp);
	tmp = EC_POINT_tate_pair(group,eblock.GetP(),sk.GetD(),ctx);
	CF_mul(value,eblock.GetC(),value,ctx);
	CF_div(value,value,tmp,ctx);

	CF_get_values(value,NULL,r,NULL);

	BN_free(p);
	BN_free(order);
	CF_free(value);
	CF_free(tmp);
	BN_CTX_free(ctx);
	
	return r;

}
ABCEBLOCK ABC::Encrypt(BIGNUM *m,TiXmlElement *policy)
{ 
       
	BN_CTX *ctx = BN_CTX_new();
    BIGNUM *order = BN_new(); 
    BIGNUM * s = BN_new();   
    COMPLEXFILED *c = CF_new();
    EC_POINT     *P = EC_POINT_new(group);
    vector<ATRGROUP> *list = new vector<ATRGROUP>();
    vector<LEAFSTORE> *lsVector = new vector<LEAFSTORE>();

    EC_GROUP_get_order(group,order,ctx);
    const EC_POINT *g =  EC_GROUP_get0_generator(group);
    BIGNUM * p = BN_new();
    EC_GROUP_get_curve_GFp(group, p,NULL,NULL, ctx);

    BN_pseudo_rand_range(s,order); 
    EC_POINT_mul(group,P,NULL,pk.GetH(),s,ctx);
    GenerateRandomLeaf(policy,s,lsVector);
  
    BIGNUM *zero = BN_int2bn(0);
    COMPLEXFILED * mEx = CF_new();
    CF_set_values(mEx,p,m,zero,ctx);
    COMPLEXFILED * randomCF = CF_new();
    CF_mod_exp(randomCF,pk.GetE(),s,ctx);
    CF_mul(c,randomCF,mEx,ctx);

    vector<LEAFSTORE>::iterator iter;
  
    for (iter=lsVector->begin();iter!=lsVector->end();iter++)  
    {
    	char *name = new char [iter->name.size()+1];
    	strcpy(name,iter->name.c_str());
       	EC_POINT* hash = String2Point(name);
       	EC_POINT* c1 = EC_POINT_new(group);
       	EC_POINT* c2 = EC_POINT_new(group);

       	EC_POINT_mul(group,c1,NULL,g,iter->value,ctx);
       	EC_POINT_mul(group,c2,NULL,hash,iter->value,ctx);
       
       	ATRGROUP atrGroup = ATRGROUP(group,iter->name,c1,c2);
       	list->push_back(atrGroup);

       	EC_POINT_free(hash);
       	EC_POINT_free(c1);
       	EC_POINT_free(c2);
    }
    string policyString = xml2string(element_wrap(policy)); 
    ABCEBLOCK eblock = ABCEBLOCK(group,c,P,list,policyString);
    BN_free(order);
    BN_free(p);
    BN_free(s);
    BN_free(zero);
    CF_free(c);
    CF_free(mEx);
    CF_free(randomCF);
    EC_POINT_free(P);
    BN_CTX_free(ctx);

    return eblock;
}
bool ABC::ContainSet(vector<string> *atrs)
{
	vector<string> *names =sk.GetSKAtrNames();
    vector<string>::iterator iter;
    for (iter=atrs->begin();iter!=atrs->end();iter++)
    {
		if(!contain(names,*iter))
	    	return false;
    } 
    return true;
}

ABCSK ABC::Delegate(vector<string> *atrs)
{
	if(!ContainSet(atrs))
    {
		return ABCSK();
    }
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *order = BN_new();
    EC_GROUP_get_order(group,order,ctx);
    const EC_POINT *g =  EC_GROUP_get0_generator(group);

    BIGNUM * r = BN_new();
    BN_pseudo_rand_range(r,order);
    EC_POINT *gr = EC_POINT_new(group);
    EC_POINT_mul(group,gr,NULL,g,r,ctx);
    
    EC_POINT *d = EC_POINT_new(group);
    EC_POINT_mul(group,d,NULL,pk.GetF(),r,ctx);  
    EC_POINT_add(group,d,d,sk.GetD(),ctx);

    EC_POINT *tmp = EC_POINT_new(group);
    vector<ATRGROUP> *list = new vector<ATRGROUP>();
    vector<string>::iterator iter;
  
    for (iter=atrs->begin();iter!=atrs->end();iter++)  
    {
    	char *name = new char [iter->size()+1];
       	strcpy(name,iter->c_str());
       	EC_POINT* hash = String2Point(name);

       	EC_POINT *c1 = EC_POINT_new(group);
       	EC_POINT *c2 = EC_POINT_new(group); 

       	BIGNUM * rn = BN_new();
       	BN_pseudo_rand_range(rn,order);
  
       	EC_POINT_mul(group,tmp,NULL,hash,rn,ctx);

       	EC_POINT_add(group,c1,tmp,gr,ctx);        
       	EC_POINT_mul(group,c2,NULL,g,rn,ctx);

       	ATRGROUP atrGroup = sk.GetAGByName(*iter);
       
       	EC_POINT_add(group,c1,c1,atrGroup.GetC1(),ctx);
       	EC_POINT_add(group,c2,c2,atrGroup.GetC2(),ctx);

       	atrGroup = ATRGROUP(group,*iter,c1,c2);
       	list->push_back(atrGroup);

       	EC_POINT_free(c1);
       	EC_POINT_free(c2);
       	EC_POINT_free(hash);
       	BN_free(rn);
    }
    
    ABCSK delegateSK = ABCSK(group,d,list);

    BN_free(order);
    BN_free(r);
    EC_POINT_free(tmp);
    EC_POINT_free(d);
    BN_CTX_free(ctx);

    return delegateSK;
}
string ABC::Integer2String(int value,int len)
{
	char buf[30];
	itoa(value,buf,2);
	string valueString(buf);
	while(valueString.size()<len)
		valueString = "0" + valueString;
	return valueString;
}

void ABC::HandleAtrs(vector<string> *atrs)
{
	vector<string> store = vector<string>();
	vector<string>::iterator iter;
	boost::regex expression("(.+):([0-9]+)=(.+)");
	boost::smatch what;
    for (iter=atrs->begin();iter!=atrs->end();iter++) 
    {

		if(boost::regex_search(*iter, what, expression))
		{
			int len = str2int(what[2]);
			string value = Integer2String(str2int(what[3]),len);
		
			for(int i=0;i<len;i++)
			{
				string init(len,'*');
				init.replace(i,1,1,value[i]);
				init = what[1]+":"+init;
				store.push_back(init);
			}
			
			atrs->erase(iter--);
		}
    
    }
    
    for(iter=store.begin();iter!=store.end();iter++)
    {
    	atrs->push_back(*iter);	
    }

}
ABCSK ABC::GenerateABCSK(vector<string> *atrs)
{
	BN_CTX *ctx = BN_CTX_new();
    BIGNUM *order = BN_new();
    EC_GROUP_get_order(group,order,ctx);
    const EC_POINT *g =  EC_GROUP_get0_generator(group);

    BIGNUM * r = BN_new();
    BN_pseudo_rand_range(r,order);
 
    
    EC_POINT *gr = EC_POINT_new(group);
    EC_POINT_mul(group,gr,NULL,g,r,ctx);
    
    BIGNUM *aInverse = BN_new();
    BN_mod_inverse(aInverse,mk.GetA(),order,ctx);

    EC_POINT * tmp = EC_POINT_new(group);
    EC_POINT_add(group,tmp,gr,mk.GetB(), ctx);
  
    EC_POINT *d = EC_POINT_new(group);
    EC_POINT_mul(group,d,NULL,tmp,aInverse,ctx);  
 
 	HandleAtrs(atrs);
    vector<ATRGROUP> *list = new vector<ATRGROUP>();
    vector<string>::iterator iter;
    for (iter=atrs->begin();iter!=atrs->end();iter++) 
    {
    	char *name = new char [iter->size()+1];
       	strcpy(name,iter->c_str());
       	EC_POINT* hash = String2Point(name);
       	EC_POINT *c1 = EC_POINT_new(group);
       	EC_POINT *c2 = EC_POINT_new(group); 

       	BIGNUM * rn = BN_new();
       	BN_pseudo_rand_range(rn,order);
  
       	EC_POINT_mul(group,tmp,NULL,hash,rn,ctx);

       	EC_POINT_add(group,c1,tmp,gr,ctx);        
       	EC_POINT_mul(group,c2,NULL,g,rn,ctx);
       	ATRGROUP atrGroup = ATRGROUP(group,*iter,c1,c2);
       	list->push_back(atrGroup);
       	BN_free(rn);
    }
    ABCSK sk = ABCSK(group,d,list);
    BN_free(order);
    BN_free(r);
    BN_free(aInverse);
    EC_POINT_free(tmp);
    EC_POINT_free(d);
    BN_CTX_free(ctx);
   
    
    return sk;

}

LEAFSTORE ABC::InitLeafStore(string name ,BIGNUM *s)
{
	LEAFSTORE store ;
	store.name = name;
	store.value = BN_new();
	BN_copy(store.value,s);
	return store;
}

void ABC::GenerateRandomLeaf(TiXmlElement *root,BIGNUM * s,vector<LEAFSTORE> *storeV)
{
	BN_CTX *ctx = BN_CTX_new();
   	BIGNUM *order = BN_new();
   	EC_GROUP_get_order(group,order,ctx);

   	Node node = Node(root);

   	if(node.IsLeaf())
   	{
    	LEAFSTORE store = InitLeafStore(node.GetValue(),s);  
     	storeV->push_back(store);
      	return ; 
   	}
 
   	Polynomial polynomial = Polynomial(s,order,atoi(node.GetValue().c_str()));
  
   	TiXmlElement * child = root->FirstChildElement();

   	while(child)
   	{
    	Node nodeChild = Node(child);
      	BIGNUM *x =BN_int2bn(nodeChild.GetPosition());

      	BIGNUM *s = polynomial.GetFunctionValue(x);
    
      	GenerateRandomLeaf(child,s,storeV);
      	child = child->NextSiblingElement();
      	BN_free(x);
      	BN_free(s);   
   	}

   	BN_free(order);
   	BN_CTX_free(ctx);
  
}

ABCMK ABC::GetMK()
{
	return mk;
}
ABCSK ABC::GetSK()
{
	return sk;
}
ABCPK ABC::GetPK()
{
	return pk;
}

void ABC::SetMK(ABCMK &mk)
{
	this->mk = mk;
  	EC_GROUP_copy(this->group,mk.GetGroup());;
}
void ABC::SetPK(ABCPK &pk)
{
	this->pk = pk; 
  	EC_GROUP_copy(this->group,pk.GetGroup());
}
void ABC::SetSK(ABCSK &sk)
{
	this->sk = sk;
  	EC_GROUP_copy(this->group,sk.GetGroup());
}
int ABC::EncryptFile(char *szSrc,char *szTarget,ABCEBLOCK &eblock,TiXmlElement *policy)
{
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *order = BN_new();
    BIGNUM *rnd = BN_new();
    EC_GROUP_get_order(group,order,ctx);;
    BN_rand_range(rnd,order);
    char * key = BN_bn2dec(rnd); 

    eblock =  Encrypt(rnd,policy);

    int state = aes_enc_file(szSrc,szTarget,(unsigned char *)key,128);
    BN_free(order);
    BN_free(rnd);
    BN_CTX_free(ctx);

    return state;
}
int ABC::DecryptFile(char *szSrc,char *szTarget,ABCEBLOCK &eblock)
{
    BIGNUM *rnd = Decrypt(eblock);
    char *key = BN_bn2dec(rnd);

    int state = aes_dec_file (szSrc,szTarget ,(unsigned char*)key,128);
    BN_free(rnd);

    return state;
}

ABC::~ABC()
{
	EC_GROUP_free(group);
	pk.~ABCPK();
	mk.~ABCMK();
	sk.~ABCSK();
}
