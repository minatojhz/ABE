#ifndef HEAD_XML
#define HEAD_XML

#include <iostream> 
#include <string>
#include <stdlib.h>
#include <tinyxml/tinystr.h>
#include <tinyxml/tinyxml.h>
#include "tate.h"
#include "str.h"

void init_node(TiXmlElement *element,
		int type,
		int pos,
		string value);

string xml2string(TiXmlDocument * doc);

TiXmlDocument * string2xml(string str);

TiXmlDocument * element_wrap(TiXmlElement *element);

TiXmlElement *element_unwrap(TiXmlDocument *doc);

TiXmlElement *BN_2xml(BIGNUM *p);

BIGNUM *xml_2BN(TiXmlElement * xml);

TiXmlElement *EC_POINT_2xml(EC_POINT * p,EC_GROUP *group,BN_CTX *ctx);

EC_POINT * xml_2EC_POINT(TiXmlElement * xml,EC_GROUP *group,BN_CTX *ctx);

TiXmlElement *CF_2xml(COMPLEXFILED *cf);

COMPLEXFILED * xml_2CF(TiXmlElement *xml,BN_CTX *ctx);



#endif
