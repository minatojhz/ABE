#ifndef HEAD_ABCPK
#define HEAD_ABCPK

#include "tate.h"
#include "str.h"
#include "xml.h"

class ABCPK
{
	private:
		//data
		EC_GROUP *group;
  		EC_POINT *h;
		EC_POINT *f;
  		COMPLEXFILED *e;
  		
  		//function
		void InitGroup(EC_GROUP *group);
		void InitH(EC_POINT *h);
		void InitE(COMPLEXFILED *e);
		void InitF(EC_POINT *f);
	public:
		ABCPK(EC_GROUP *group,EC_POINT *h,EC_POINT *f,COMPLEXFILED *e);
		/**********************************************************************
		函数名称：ABCPK
		函数功能：ABCPK构造函数　xml构造
		参数说明：
			group: EC_GROUP *,椭圆曲线群
			element:TiXmlElement * xml的结构化ABCPK
		************************************************************************/	
		ABCPK(EC_GROUP *group,TiXmlElement *element);
	    ABCPK();
	    ABCPK(const ABCPK &AP);
	    
		EC_POINT * GetH();
		COMPLEXFILED *GetE();
		EC_POINT * GetF();
		EC_GROUP *GetGroup();

		void SetH(EC_POINT * h);
		void SetE(COMPLEXFILED *e);
		void SetGroup(EC_GROUP * group);
		void SetF(EC_POINT *f);
		
		//转化成结构化xml
		TiXmlElement * ToTiXmlElement();
		
		ABCPK & operator= (const ABCPK &pk);
		friend ostream& operator<<(ostream &os,ABCPK &pk);
		
		~ABCPK();
		bool IsNULL();
};

#endif
