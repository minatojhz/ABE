#ifndef HEAD_ABCEBLOCK
#define HEAD_ABCEBLOCK

#include "tate.h"
#include "str.h"
#include "xml.h"
#include "ATRGROUP.h"

class ABCEBLOCK
{  
	private:
		//data
		EC_GROUP *group;//椭圆曲线群
  		COMPLEXFILED *c;//M.e(g,g)^(a.s)
 		EC_POINT     *p;//h^s
  		vector<ATRGROUP> *list;//(y,C0(y),C1(y))
  		string	policy;//
  		
  		//function
		void InitGroup(EC_GROUP *group);
		void InitC(COMPLEXFILED *c);
		void InitP(EC_POINT *p);
		void InitList(vector<ATRGROUP> *list);
		
	public:
		ABCEBLOCK(EC_GROUP *group,COMPLEXFILED *c,EC_POINT *p,vector<ATRGROUP> *list,string policy);
		
		/**********************************************************************
		函数名称：ABCEBLOCK
		函数功能：ABCEBLOCK构造函数　xml构造
		参数说明：
			group: EC_GROUP *,椭圆曲线群
			element:TiXmlElement * xml的结构化ABCEBLOCK
		************************************************************************/
		ABCEBLOCK(EC_GROUP *group,TiXmlElement *element );
		
		ABCEBLOCK();
		ABCEBLOCK(const ABCEBLOCK &AE);

		EC_POINT* GetP();
 		COMPLEXFILED* GetC();
		vector<ATRGROUP>* GetList();
		EC_GROUP* GetGroup();
		string GetPolicy();
		
		void SetP(EC_POINT *p);
		void SetC(COMPLEXFILED* c);
		void SetList( vector<ATRGROUP> *list);
		void SetGroup(EC_GROUP *group);
		void SetPolicy(string policy);
		/**********************************************************************
		函数名称：GetAGGByName
		函数功能：对list通过name的检索，返回检索值
		参数说明：
			name: string,名字字符串
		返回值：ATRGROUP ,检索失败返回NULL,否则返回检索的属性组
		************************************************************************/
		ATRGROUP GetAGByName(string name);
		
		//转化成结构化xml
		TiXmlElement *ToTiXmlElement();

		ABCEBLOCK &operator=(const ABCEBLOCK &eblock);
		
		friend ostream& operator<<(ostream &os,ABCEBLOCK &eblock);

		~ABCEBLOCK();
		bool IsNULL();


};

#endif
