#ifndef HEAD_ATRGROUP
#define HEAD_ATRGROUP

#include "tate.h"
#include "str.h"
#include "xml.h"

class ATRGROUP
{
	private:
		//data
		EC_GROUP *group;//椭圆曲线群
    	string name;//属性组属性值
    	EC_POINT *c1;
		EC_POINT *c2;
		
		//function
		void InitGroup(EC_GROUP *group);
		void InitC1(EC_POINT *c1);
		void InitC2(EC_POINT *c2);
  	public:

		ATRGROUP(EC_GROUP *group,string name,EC_POINT *c1,EC_POINT *c2);
		
		/**********************************************************************
		函数名称：ATRGROUP
		函数功能：ATRGROUP构造函数　xml构造
		参数说明：
			group: EC_GROUP *,椭圆曲线群
			element:TiXmlElement * xml的结构化ATRGROUP
		************************************************************************/	
		ATRGROUP(EC_GROUP *group,TiXmlElement *element);
		
		ATRGROUP(const ATRGROUP &A);
		ATRGROUP();

		EC_POINT * GetC1();
		EC_POINT * GetC2();
		string GetName();
		EC_GROUP *GetGroup();

		void SetGroup(EC_GROUP *group);
		void SetC1(EC_POINT *c1);
		void SetC2(EC_POINT *c2);
		void SetName(string name);

		//转化为结构化xml
		TiXmlElement* ToTiXmlElement();
		
		ATRGROUP & operator=(const ATRGROUP &);
		friend ostream& operator<<(ostream &os,ATRGROUP &atrgroup);
		~ATRGROUP();
		
		bool IsNULL();
		
};

#endif
