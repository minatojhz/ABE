#include "ABCSK.h"
ABCSK::ABCSK(EC_GROUP *group,EC_POINT *d,vector<ATRGROUP> *list)
{
	InitGroup(group);
    InitD(d);
    InitList(list);
}
ABCSK::ABCSK()
{
	d = NULL;
   	group = NULL;
   	list = NULL;
}
ABCSK::ABCSK(const ABCSK &AS)
{
  	InitGroup(AS.group);
  	InitD(AS.d);
  	InitList(AS.list);
}
ABCSK::ABCSK(EC_GROUP *group,TiXmlElement *element)
{ 
    BN_CTX *ctx = BN_CTX_new();
    InitGroup(group);
    list = new vector<ATRGROUP> ();

    TiXmlElement * child = element->FirstChildElement();
    d = xml_2EC_POINT(child,group,ctx);

    child = child->NextSiblingElement();
    child = child->FirstChildElement();
    
    while(child)
    {
        list->push_back(ATRGROUP(group,child));
        child = child->NextSiblingElement();
    } 

}

void ABCSK::InitGroup(EC_GROUP *group)
{
   	const EC_METHOD *method = EC_GFp_simple_method();
  	this->group = EC_GROUP_new(method);
   	EC_GROUP_copy(this->group,group);
}

void ABCSK::InitD(EC_POINT *d)
{
   	this->d = EC_POINT_new(this->group);
   	EC_POINT_copy(this->d,d);
}

void ABCSK::InitList(vector<ATRGROUP> *list)
{
	this->list = new vector<ATRGROUP>(); 
	vector<ATRGROUP>::iterator iter;  
	for (iter=list->begin();iter!=list->end();iter++)  
    {  
    	this->list->push_back(*iter);
   	}  
}

EC_POINT* ABCSK::GetD()
{
	EC_POINT *result = EC_POINT_new(group);
	EC_POINT_copy(result,d);
	return result;
}
vector<ATRGROUP>* ABCSK::GetList()
{
	vector<ATRGROUP>* result = new vector<ATRGROUP>(); 
	vector<ATRGROUP>::iterator iter;  
	for (iter=list->begin();iter!=list->end();iter++)  
    {  
        result->push_back(*iter);
   	}  
	return result;
}
EC_GROUP *ABCSK::GetGroup()
{
   	const EC_METHOD *method = EC_GFp_simple_method();
   	EC_GROUP  *result= EC_GROUP_new(method);
  	EC_GROUP_copy(result,group);
   	return group;
}
void ABCSK::SetD(EC_POINT *d)
{
	EC_POINT_copy(this->d,d);
}
void ABCSK::SetList( vector<ATRGROUP> *list)
{
 	this->list->clear();
	vector<ATRGROUP>::iterator iter;  
	for (iter=list->begin();iter!=list->end();iter++)  
    {  
        this->list->push_back(*iter);
   	}  

}
void ABCSK::SetGroup(EC_GROUP *group)
{
  	EC_GROUP_copy(this->group,group);
}
ATRGROUP ABCSK::GetAGByName(string name)
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
TiXmlElement *ABCSK::ToTiXmlElement()
{
    BN_CTX *ctx = BN_CTX_new();
    TiXmlElement *skXML = new TiXmlElement("privatekey");
    TiXmlElement * dXML = EC_POINT_2xml(d,group,ctx);
    TiXmlElement * listXML = new TiXmlElement("list");
    
    vector<ATRGROUP>::iterator iter;  
    for (iter=list->begin();iter!=list->end();iter++)  
    {  
        TiXmlElement *atrXML = iter->ToTiXmlElement();
        listXML->LinkEndChild(atrXML);
    }  
     
    skXML->LinkEndChild(dXML);
    skXML->LinkEndChild(listXML);
   
    return skXML;

}
vector<string>* ABCSK::GetSKAtrNames()
{
	vector<string>* atrNames = new vector<string>();
   	vector<ATRGROUP>::iterator iter;
   	for (iter=list->begin();iter!=list->end();iter++)
   	{
    	string name =  iter->GetName();
     	atrNames->push_back(name);
   	}
   	return atrNames;
}
ABCSK &ABCSK::operator=(const ABCSK &sk)
{
    InitGroup(sk.group);
    InitD(sk.d);
    InitList(sk.list);
    return *this;
}
ostream& operator<<(ostream &os,ABCSK &sk)
{
    TiXmlElement *skEle  = sk.ToTiXmlElement();
    TiXmlDocument *skDoc = element_wrap(skEle);
    string str = xml2string(skDoc); 
    os<<str;
    return os;
}
ABCSK::~ABCSK()
{
    EC_POINT_free(d);
    EC_GROUP_free(group);
    if(list != NULL)
    {
    	vector<ATRGROUP>::iterator iter;  
		for (iter=list->begin();iter!=list->end();iter++)  
    	{  
            iter->~ATRGROUP(); 
   		}  
    }
    d = NULL;
    group = NULL;
    list = NULL;
}
bool ABCSK::IsNULL()
{
	if(d == NULL 
		&&group == NULL 
		&&list == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}
