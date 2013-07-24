#ifndef HEAD_ABCMK
#define HEAD_ABCMK

#include "tate.h"
#include "str.h"
#include "xml.h"

class ABCMK
{
	private:
		//data
		EC_GROUP * group;//椭圆曲线群
		BIGNUM * a;//b
  		EC_POINT *b;//g^a
  		
  		//function
  		void InitGroup(EC_GROUP *group);
		void InitA(BIGNUM *a);
		void InitB(EC_POINT *b);
	public :
		ABCMK(EC_GROUP *group,BIGNUM *a,EC_POINT *b);
		
		/**********************************************************************
		函数名称：ABCMK
		函数功能：ABCMK构造函数　xml构造
		参数说明：
			group: EC_GROUP *,椭圆曲线群
			element:TiXmlElement * xml的结构化ABCMK
		************************************************************************/	
		ABCMK(EC_GROUP *group,TiXmlElement * element);
		
 		ABCMK();
		ABCMK(const ABCMK &AM);
	

		BIGNUM * GetA();
		EC_POINT *GetB();
		EC_GROUP *GetGroup();

		void SetA(BIGNUM * a);
		void SetB(EC_POINT *b);
		void SetGroup(EC_GROUP *group);
		//转化为结构化xml
		TiXmlElement *ToTiXmlElement();
		
	    ABCMK & operator=(const ABCMK& mk);
	    friend ostream& operator<<(ostream &os,ABCMK& mk);
	    ~ ABCMK();
	    bool IsNULL();
};

#endif
