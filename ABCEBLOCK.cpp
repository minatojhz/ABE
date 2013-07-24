#include "ABCEBLOCK.h"

ABCEBLOCK::ABCEBLOCK(EC_GROUP *group,COMPLEXFILED *c,EC_POINT *p,vector<ATRGROUP> *list,string policy)
{
	InitGroup(group);
	InitC(c);
	InitP(p);
	InitList(list);
	this->policy = policy;
}

ABCEBLOCK::ABCEBLOCK(EC_GROUP *group,TiXmlElement *element )
{
	BN_CTX *ctx = BN_CTX_new();
    list = new vector<ATRGROUP>(); 
  
    TiXmlElement * child = element->FirstChildElement();
    policy = xml2string(element_wrap(child));
    
    child = child->NextSiblingElement();
    p = xml_2EC_POINT(child,group,ctx);   

    child = child->NextSiblingElement();
    c = xml_2CF(child,ctx);

    child = child->NextSiblingElement();
    child = child->FirstChildElement();
    while(child)
    {   
        list->push_back(ATRGROUP(group,child));
        child = child->NextSiblingElement();
    }   
    
    BN_CTX_free(ctx);
}
ABCEBLOCK::ABCEBLOCK(const ABCEBLOCK &AE)
{
	InitGroup(AE.group);
	InitC(AE.c);
	InitP(AE.p);
	InitList(AE.list);
}
ABCEBLOCK::ABCEBLOCK()
{
	group = NULL;
	c = NULL;
    p = NULL;
	list = NULL;
	policy = "";
}
EC_POINT* ABCEBLOCK::GetP()
{
	EC_POINT *result = EC_POINT_new(group);
	EC_POINT_copy(result,p);

	return result;

}
COMPLEXFILED *ABCEBLOCK::GetC()
{

	COMPLEXFILED * result = CF_new();
	CF_copy(result,c);
	return result;
}

vector<ATRGROUP>* ABCEBLOCK::GetList()
{

	vector<ATRGROUP>* result = new vector<ATRGROUP>(); 

	vector<ATRGROUP>::iterator iter;  
	for (iter=list->begin();iter!=list->end();iter++)  
    {  
    	result->push_back(*iter);
   	}  
	return result;
}
EC_GROUP *ABCEBLOCK::GetGroup()
{

   	const EC_METHOD *method = EC_GFp_simple_method();
   	EC_GROUP  *result= EC_GROUP_new(method);
   	EC_GROUP_copy(result,group);

}

void ABCEBLOCK::InitGroup(EC_GROUP *group)
{
	const EC_METHOD *method = EC_GFp_simple_method();
   	this->group = EC_GROUP_new(method);
   	EC_GROUP_copy(this->group,group);
}

void ABCEBLOCK::InitC(COMPLEXFILED *c)
{
   	this->c = CF_new();
   	CF_copy(this->c,c);
}

void ABCEBLOCK::InitP(EC_POINT *p)
{
   	this->p = EC_POINT_new(this->group);
   	EC_POINT_copy(this->p,p);
}

void ABCEBLOCK::InitList(vector<ATRGROUP> *list)
{
	this->list = new vector<ATRGROUP>(); 
	vector<ATRGROUP>::iterator iter;  
	for (iter=list->begin();iter!=list->end();iter++)  
    {  
    	this->list->push_back(*iter);
   	}  
}


void ABCEBLOCK::SetP(EC_POINT *p)
{
	EC_POINT_copy(this->p,p);
}
void ABCEBLOCK::SetList( vector<ATRGROUP> *list)
{

	this->list->clear();
	vector<ATRGROUP>::iterator iter;  
	for (iter=list->begin();iter!=list->end();iter++)  
    {  
        this->list->push_back(*iter);
   	}  

}
void ABCEBLOCK::SetGroup(EC_GROUP *group)
{
	EC_GROUP_copy(this->group,group);
}

void ABCEBLOCK::SetC(COMPLEXFILED *c)
{
	if(IsNULL())
		return ;
	CF_copy(this->c,c);
}
void  ABCEBLOCK::SetPolicy(string policy)
{
	this->policy = policy;
}

string ABCEBLOCK::GetPolicy()
{
	return this->policy;
}
ATRGROUP ABCEBLOCK::GetAGByName(string name)
{
	vector<ATRGROUP>::iterator iter;  
    for (iter=list->begin();iter!=list->end();iter++)  
    {  
        string atr = iter->GetName();
		if(atr == name) 
	 	{
	    	return *iter;
	 	}
    }  
	return ATRGROUP();
}
TiXmlElement *ABCEBLOCK::ToTiXmlElement()
{
	BN_CTX *ctx = BN_CTX_new();
	
   	TiXmlElement *rootXML = new TiXmlElement("abceblock");
   	TiXmlElement *policyXML = element_unwrap(string2xml(policy));
   	TiXmlElement *pXML = EC_POINT_2xml(p,group,ctx); 
   	TiXmlElement *cXML = CF_2xml(c);
	TiXmlElement * listXML = new TiXmlElement("list"); 
  
	vector<ATRGROUP>::iterator iter;  
   	for (iter=list->begin();iter!=list->end();iter++)  
   	{  
        TiXmlElement *atrXML = iter->ToTiXmlElement();
        listXML->LinkEndChild(atrXML);
   	}  
	rootXML->LinkEndChild(policyXML);
   	rootXML->LinkEndChild(pXML);
   	rootXML->LinkEndChild(cXML); 
   	rootXML->LinkEndChild(listXML);

   	BN_CTX_free(ctx);
   	return rootXML;
}

ABCEBLOCK& ABCEBLOCK::operator=(const ABCEBLOCK &eblock)
{	
	InitGroup(eblock.group);
	InitC(eblock.c);
	InitP(eblock.p);
	InitList(eblock.list);
	this->policy = eblock.policy;
	return *this;
}
ostream& operator<<(ostream &os,ABCEBLOCK &eblock)
{
	TiXmlElement *blockEle = eblock.ToTiXmlElement();
	TiXmlDocument *blockDoc = element_wrap(blockEle);
	string str = xml2string(blockDoc); 
	os<<str;
	return os;
}
ABCEBLOCK::~ABCEBLOCK()
{
	
	EC_POINT_free(p);
	CF_free(c);
	EC_GROUP_free(group);
	if(list != NULL)
	{
    	vector<ATRGROUP>::iterator iter;  
	    for (iter=list->begin();iter!=list->end();iter++)  
    	{  
        	iter->~ATRGROUP(); 
   	    }  
    }
	c = NULL;
	p = NULL;
	group = NULL;
	list = NULL;
	policy = "";
}
bool ABCEBLOCK::IsNULL()
{
	if(c == NULL 
		&&p == NULL
		&&group == NULL 
		&&list == NULL
		&&policy == "")
	{
		return true;
	}
	else
	{
		return false;
	}

}
