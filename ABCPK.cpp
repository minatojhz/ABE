#include "ABCPK.h"

ABCPK::ABCPK(EC_GROUP *group,EC_POINT *h,EC_POINT *f,COMPLEXFILED *e)
{
	InitGroup(group);
   	InitH(h);
   	InitF(f);
   	InitE(e);
}

ABCPK::ABCPK(EC_GROUP *group,TiXmlElement *element)
{
   	BN_CTX *ctx = BN_CTX_new();
   	InitGroup(group);
   	TiXmlElement * child = element->FirstChildElement(); 
   	h = xml_2EC_POINT(child,group,ctx);
   	child = child->NextSiblingElement();
   	f = xml_2EC_POINT(child,group,ctx);
   	e = xml_2CF(child->NextSiblingElement(),ctx);
}
ABCPK::ABCPK()
{
   	group = NULL;
   	h = NULL;
   	f = NULL;
   	e = NULL;
}
ABCPK::ABCPK(const ABCPK &AP)
{
   	InitGroup(AP.group);
   	InitH(AP.h);
   	InitF(AP.f);
   	InitE(AP.e);
}
void ABCPK::InitH(EC_POINT *h)
{
   	this->h = EC_POINT_new(this->group);
   	EC_POINT_copy(this->h,h);
}

void ABCPK::InitE(COMPLEXFILED *e)
{
   	this->e = CF_new();
   	CF_copy(this->e,e);
}
void ABCPK::InitF(EC_POINT *f)
{
   	this->f = EC_POINT_new(this->group);
   	EC_POINT_copy(this->f,f);
}
void ABCPK::InitGroup(EC_GROUP *group)
{
   	const EC_METHOD *method = EC_GFp_simple_method();
   	this->group = EC_GROUP_new(method);
   	EC_GROUP_copy(this->group,group);
}

EC_POINT * ABCPK::GetH()
{
	EC_POINT *result = EC_POINT_new(group);
	EC_POINT_copy(result,h);
	return result;
}

EC_POINT *ABCPK::GetF()
{
	EC_POINT *result = EC_POINT_new(group);
	EC_POINT_copy(result,f);
	return f;
}
COMPLEXFILED *ABCPK::GetE()
{
	COMPLEXFILED * result = CF_new();
	CF_copy(result,e);
	return e;
}

EC_GROUP *ABCPK::GetGroup()
{
   	const EC_METHOD *method = EC_GFp_simple_method();
   	EC_GROUP  *result= EC_GROUP_new(method);
   	EC_GROUP_copy(result,group);
   	return result;
}

void ABCPK::SetH(EC_POINT * h)
{
	EC_POINT_copy(this->h,h);
}
void ABCPK::SetE(COMPLEXFILED *e)
{
	CF_copy(this->e,e);

}
void ABCPK::SetGroup(EC_GROUP * group)
{
	EC_GROUP_copy(this->group,group);
}

void ABCPK::SetF(EC_POINT *f)
{
	EC_POINT_copy(this->f,f);
}
TiXmlElement * ABCPK::ToTiXmlElement()
{
    BN_CTX *ctx = BN_CTX_new();
    TiXmlElement *pkXML = new TiXmlElement("publickey");

    TiXmlElement * hXML = EC_POINT_2xml(h,group,ctx);
    TiXmlElement * fXML = EC_POINT_2xml(f,group,ctx);
    TiXmlElement * eXML = CF_2xml(e);

    pkXML->LinkEndChild(hXML);
    pkXML->LinkEndChild(fXML);
    pkXML->LinkEndChild(eXML);
    
    return pkXML;
}
ABCPK & ABCPK::operator= (const ABCPK &pk)
{
   	InitGroup(pk.group);
   	InitH(pk.h);
   	InitF(pk.f);
   	InitE(pk.e);
   	return *this;
}
ostream& operator<<(ostream &os,ABCPK &pk)
{
   	TiXmlElement *pkEle = pk.ToTiXmlElement();
   	TiXmlDocument *pkDoc = element_wrap(pkEle);
   	string str = xml2string(pkDoc); 
   	os<<str;
   	return os;
}
ABCPK::~ABCPK()
{
   	EC_GROUP_free(group);
   	EC_POINT_free(h);
   	EC_POINT_free(f);
  	CF_free(e);
   	group = NULL;
   	h = NULL;
   	f = NULL;
   	e = NULL;
}
bool ABCPK::IsNULL()
{
	if(group == NULL 
		&&h == NULL 
		&&f == NULL
		&&e == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}
