#include "xml.h"

string xml2string(TiXmlDocument * doc)
{
  TiXmlPrinter printer;
  doc->Accept(&printer);
  return printer.CStr();
}

TiXmlDocument * string2xml(string str)
{
  TiXmlDocument *doc = new TiXmlDocument();
  doc->Parse(str.data());
  return doc;
}
TiXmlDocument * element_wrap(TiXmlElement *element)
{
  TiXmlDocument *doc = new TiXmlDocument();
  TiXmlElement *root = (TiXmlElement *)element->Clone();
  doc->LinkEndChild(root);
  return doc;
}

TiXmlElement *element_unwrap(TiXmlDocument *doc)
{
  return (TiXmlElement *)doc->RootElement()->Clone();
}
TiXmlElement *BN_2xml(BIGNUM *p)
{
	char *value = BN_bn2hex(p);
	TiXmlElement *bn_xml = new TiXmlElement("bn");
	bn_xml->SetAttribute("value",value);
	return bn_xml;
}
BIGNUM *xml_2BN(TiXmlElement * xml)
{
	TiXmlAttribute *a = xml->FirstAttribute();
	BIGNUM *bn = BN_str2bn(a->Value(),16);
	return bn;
}
TiXmlElement *EC_POINT_2xml(EC_POINT * p,EC_GROUP *group,BN_CTX *ctx)
{
    BIGNUM *px = BN_new();
    BIGNUM *py = BN_new();
    EC_POINT_get_affine_coordinates_GFp(group,p,px,py,ctx);
    char *px_hex = BN_bn2hex(px);
    char *py_hex = BN_bn2hex(py);
    TiXmlElement* ec_point_xml = new TiXmlElement("ec");
    ec_point_xml->SetAttribute("x",px_hex);
    ec_point_xml->SetAttribute("y",py_hex);

    return ec_point_xml;
}

EC_POINT * xml_2EC_POINT(TiXmlElement * xml,EC_GROUP *group,BN_CTX *ctx)
{
   TiXmlAttribute *xa = xml-> FirstAttribute();  
   TiXmlAttribute *ya = xa->Next();
   
   BIGNUM * x = BN_str2bn(xa->Value(),16);
   BIGNUM * y = BN_str2bn(ya->Value(),16);

   EC_POINT * ec_point = EC_POINT_new(group);
   EC_POINT_set_affine_coordinates_GFp(group,ec_point,x,y,ctx);
   
   return ec_point;


}

TiXmlElement *CF_2xml(COMPLEXFILED *cf)
{
  BIGNUM *p = BN_new();
  BIGNUM *r = BN_new();
  BIGNUM *i = BN_new();

  CF_get_values(cf ,p,r,i);
  
  char *p_hex  = BN_bn2hex(p);
  char *i_hex  = BN_bn2hex(i);
  char *r_hex  = BN_bn2hex(r);

  TiXmlElement *cf_xml  = new TiXmlElement("cf");

  cf_xml->SetAttribute("p",p_hex);
  cf_xml->SetAttribute("r",r_hex);
  cf_xml->SetAttribute("i",i_hex);
 
  return cf_xml;

}

COMPLEXFILED * xml_2CF(TiXmlElement *xml,BN_CTX *ctx)
{
   TiXmlAttribute *pa = xml-> FirstAttribute();
   TiXmlAttribute *ra = pa->Next();
   TiXmlAttribute *ia = ra->Next();

   BIGNUM * p = BN_str2bn(pa->Value(),16);
   BIGNUM * r = BN_str2bn(ra->Value(),16);
   BIGNUM * i = BN_str2bn(ia->Value(),16);
   
   COMPLEXFILED *cf = CF_new();
   CF_set_values(cf ,p,r,i,ctx);
   
   return cf;

}

void init_node(TiXmlElement *element,
               int type,
               int pos,
               string value)
{
    string name = type ? "node":"leaf";
    string valtype  = type ? "threhold":"value"; 
    string posstr = int2str(pos);

    element->SetValue(name.c_str());
    element->SetAttribute("pos",posstr.c_str());
    element->SetAttribute(valtype.c_str(),value.c_str());
}
