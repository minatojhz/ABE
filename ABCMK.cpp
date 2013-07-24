#include "ABCMK.h"
ABCMK::ABCMK(EC_GROUP *group,BIGNUM *a,EC_POINT *b)
{
	InitGroup(group);
   	InitA(a);
   	InitB(b);
}

ABCMK::ABCMK(EC_GROUP *group,TiXmlElement * element)
{
   	BN_CTX *ctx = BN_CTX_new();
   	InitGroup(group);
   	TiXmlElement *child = element->FirstChildElement();
   	a = xml_2BN(child);
   	child = child->NextSiblingElement();
   	b = xml_2EC_POINT(child,group,ctx);
}
ABCMK::ABCMK()
{
	group = NULL;
	a = NULL;
  	b = NULL;
}
ABCMK::ABCMK(const ABCMK &AM)
{
  	InitGroup(AM.group);
  	InitA(AM.a);
  	InitB(AM.b);
}
void ABCMK::InitGroup(EC_GROUP *group)
{
   	const EC_METHOD *method = EC_GFp_simple_method();
   	this->group = EC_GROUP_new(method);
   	EC_GROUP_copy(this->group,group);
}
void ABCMK::InitA(BIGNUM *a)
{
   	this->a = BN_new();
   	BN_copy(this->a,a);
}
void ABCMK::InitB(EC_POINT *b)
{
   this->b = EC_POINT_new(this->group);
   EC_POINT_copy(this->b,b);
}

BIGNUM *ABCMK::GetA()
{
	BIGNUM *result = BN_new();
	BN_copy(result,a);
	return result;
}

EC_POINT *ABCMK::GetB()
{
   	EC_POINT *result = EC_POINT_new(group);
   	EC_POINT_copy(result,b);
   	return result;
}

EC_GROUP *ABCMK::GetGroup()
{
   	const EC_METHOD *method = EC_GFp_simple_method();
   	EC_GROUP  *result= EC_GROUP_new(method);
   	EC_GROUP_copy(result,group);
  	return result;
}
void ABCMK::SetGroup(EC_GROUP * group)
{
	EC_GROUP_copy(this->group,group);
}
void ABCMK::SetB(EC_POINT *b)
{
	EC_POINT_copy(this->b,b);
}
void ABCMK::SetA(BIGNUM *a)
{
	BN_copy(this->a,a);
}

TiXmlElement *ABCMK::ToTiXmlElement()
{
  	BN_CTX *ctx = BN_CTX_new();
  	TiXmlElement *mk = new TiXmlElement("masterkey");
  	TiXmlElement *aXML = BN_2xml(this->a);
  	TiXmlElement *bXML  = EC_POINT_2xml(this->b,group,ctx);
  	mk->LinkEndChild(aXML);
  	mk->LinkEndChild(bXML);
  	BN_CTX_free(ctx);
  	return mk;
}

ABCMK & ABCMK::operator=(const ABCMK& mk)
{
	InitGroup(mk.group);
	InitA(mk.a);
	InitB(mk.b);
	return *this;
}
ostream& operator<<(ostream &os,ABCMK& mk)
{
	TiXmlElement *mkEle = mk.ToTiXmlElement();
	TiXmlDocument *mkDoc = element_wrap(mkEle);
	string str = xml2string(mkDoc); 
	os<<str;
	return os;
}
ABCMK::~ ABCMK()
{
    EC_GROUP_free(group);
    BN_free(a);
    EC_POINT_free(b);
    group = NULL;
    a = NULL;
    b = NULL;
}

bool ABCMK::IsNULL()
{
	if(group == NULL 
		&&a == NULL 
		&&b == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}
