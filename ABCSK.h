#ifndef HEAD_ABCSK
#define HEAD_ABCSK

#include "tate.h"
#include "str.h"
#include "xml.h"
#include "ATRGROUP.h"

class ABCSK
{
	private:
		//data
		EC_GROUP *group;//椭圆曲线群
	 	EC_POINT *d;//g^((a+r)/b)
		vector<ATRGROUP> *list;////(y,C0(y),C1(y))
		//function
		void InitGroup(EC_GROUP *group);
		void InitD(EC_POINT *d);
		void InitList(vector<ATRGROUP> *list);
		
	public:
		ABCSK(EC_GROUP *group,EC_POINT *d,vector<ATRGROUP> *list);
		/**********************************************************************
		函数名称：ABCSK
		函数功能：ABCSK构造函数　xml构造
		参数说明：
			group: EC_GROUP *,椭圆曲线群
			element:TiXmlElement * xml的结构化ABCSK
		************************************************************************/		
		ABCSK(EC_GROUP *group,TiXmlElement *element);	
		ABCSK();
		ABCSK(const ABCSK &AS);
		
		EC_POINT* GetD();
		vector<ATRGROUP>* GetList();
		EC_GROUP* GetGroup();

		void SetD(EC_POINT *d);
		void SetList( vector<ATRGROUP> *list);
		void SetGroup(EC_GROUP *group);
		
		/**********************************************************************
		函数名称：GetAGGByName
		函数功能：对list通过name的检索，返回检索值
		参数说明：
			name: string,名字字符串
		返回值：ATRGROUP ,检索失败返回NULL,否则返回检索的属性组
		************************************************************************/
		ATRGROUP GetAGByName(string name);
		
		
		/**********************************************************************
		函数名称：GetSKAtrNames
		函数功能：获取私钥对应的属性集合
		参数说明：
			无
		返回值：vector<string>*,属性集合
		************************************************************************/
        vector<string>* GetSKAtrNames();
        
        //转化成结构化xml
		TiXmlElement *ToTiXmlElement();

		 ABCSK &operator=(const ABCSK &sk);
		 friend ostream& operator<<(ostream &os,ABCSK &sk);
		 
		 ~ABCSK();
		 
		 bool IsNULL();


};

#endif
