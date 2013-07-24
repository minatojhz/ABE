#include "ATRGROUP.h"
ATRGROUP::ATRGROUP(EC_GROUP *group,string name,EC_POINT *c1,EC_POINT *c2)
{
	InitGroup(group);
	InitC1(c1);
	InitC2(c2);
	this->name = name;
}

ATRGROUP::ATRGROUP()
{
   	c1 = NULL;
   	c2 = NULL;
   	group = NULL;
   	name = "";
}

ATRGROUP::ATRGROUP(EC_GROUP *group,TiXmlElement *element)
{
	BN_CTX *ctx = BN_CTX_new();
   	InitGroup(group);
   	TiXmlAttribute *nameXML = element-> FirstAttribute();
   	this->name = nameXML->Value();

   	TiXmlElement * child = element->FirstChildElement();
   	c1  = xml_2EC_POINT(child,group,ctx);
   	c2  = xml_2EC_POINT(child->NextSiblingElement(),group,ctx);
   	
   	BN_CTX_free(ctx);
}

ATRGROUP::ATRGROUP(const ATRGROUP &A)
{
    InitGroup(A.group);
  	InitC1(A.c1);
  	InitC2(A.c2);
  	this->name = A.name;
}


void ATRGROUP::InitGroup(EC_GROUP *group)
{
	const EC_METHOD *method = EC_GFp_simple_method();
   	this->group = EC_GROUP_new(method);
   	EC_GROUP_copy(this->group,group);
}
void ATRGROUP::InitC1(EC_POINT *c1)
{
   	this->c1 = EC_POINT_new(this->group);
   	EC_POINT_copy(this->c1,c1);

}
void ATRGROUP::InitC2(EC_POINT *c2)
{
   	this->c2 = EC_POINT_new(this->group);
   	EC_POINT_copy(this->c2,c2);
}


EC_POINT * ATRGROUP::GetC1()
{
	EC_POINT *result = EC_POINT_new(group);
	EC_POINT_copy(result,c1);
	return result;
}
EC_POINT * ATRGROUP::GetC2()
{
	EC_POINT *result = EC_POINT_new(group);
	EC_POINT_copy(result,c2);
	return result;
}

string ATRGROUP::GetName()
{
	return name;
}
EC_GROUP *ATRGROUP::GetGroup()
{
	const EC_METHOD *method = EC_GFp_simple_method();
   	EC_GROUP  *result= EC_GROUP_new(method);
   	EC_GROUP_copy(result,group);
}

void ATRGROUP::SetGroup(EC_GROUP *group)
{
 	EC_GROUP_copy(this->group,group);
}
void ATRGROUP::SetC1(EC_POINT *c1)
{
  	EC_POINT_copy(this->c1,c1);
}
void ATRGROUP::SetC2(EC_POINT *c2)
{
  	EC_POINT_copy(this->c2,c2);
}
void ATRGROUP::SetName(string name)
{
  	this->name = name ;
}

TiXmlElement* ATRGROUP::ToTiXmlElement()
{
	BN_CTX *ctx = BN_CTX_new();

    TiXmlElement *atrGXML = new TiXmlElement("atrgroup"); 
    
    atrGXML->SetAttribute("name",name.c_str());

    
    TiXmlElement * c1XML = EC_POINT_2xml(c1,group,ctx);    
    TiXmlElement * c2XML = EC_POINT_2xml(c2,group,ctx);

    atrGXML->LinkEndChild(c1XML);
    atrGXML->LinkEndChild(c2XML);

    return atrGXML;

}

ATRGROUP & ATRGROUP::operator=(const ATRGROUP &atrgroup)
{
    InitGroup(atrgroup.group);
    this->name = atrgroup.name;
    InitC1(atrgroup.c1);
    InitC2(atrgroup.c2);
    return *this;
}
ostream& operator<<(ostream &os,ATRGROUP &atrgroup)
{
    TiXmlElement *atrgroupEle  = atrgroup.ToTiXmlElement();
    TiXmlDocument *atrgroupDoc = element_wrap(atrgroupEle);
    string str = xml2string(atrgroupDoc); 
    os<<str;
    return os;
}

ATRGROUP::~ATRGROUP()
{
   	EC_POINT_free(c1);
   	EC_POINT_free(c2);
   	EC_GROUP_free(group);
   	group = NULL;
   	c1 = NULL;
   	c2 = NULL;
   	name = "";
}


bool ATRGROUP::IsNULL()
{
   	if(c1 == NULL 
   		&&c2 == NULL
   		&&group== NULL
   		&&name == "")
   {
		return true;
   }
   else 
        return false;

}
