#ifndef HEAD_ACCESSTREE
#define HEAD_ACCESSTREE

#include "tate.h"
#include "str.h"
#include "xml.h"

#include "ATRPATHLCM.h"
#include "Node.h"
#include "Polynomial.h"

class AccessTree
{
	private:
		//data
		string accessTree;//访问树xml字符串
		//function
		bool SatisfyAceessTree(vector<string> *atrs,TiXmlElement* element,deque<Node>* pNodeQue);

		void GetPartyIndex(stack<Node>* ps,vector<int>* pv,int threshold);

		void UpdateATRPATHLCM(vector<ATRPATHLCM>* pv,stack<Node>* ps,int threshold,int track,BIGNUM* p);
	public :
	
		AccessTree(string accessTree);
		
		/**********************************************************************
		函数名称：AccessTree
		函数功能：AccessTree构造函数　xml构造
		参数说明：
			element:TiXmlElement * xml的结构化AccessTree
		************************************************************************/	
		AccessTree(TiXmlElement *element);
		/**********************************************************************
		函数名称：IsSatisfied
		函数功能：判断属性集合是否满足访问树
		参数说明：
			atrs:vector<string>*  属性集合
			pNodeQue：deque<Node>* 生成满足属性队列
		返回值:属性满足访问树返回true,否则返回false
		************************************************************************/	
		bool IsSatisfied(vector<string>* atrs,deque<Node>* pNodeQue);
		
		/**********************************************************************
		函数名称：GetATRPATHLCM
		函数功能：获取匹配属性对应拉格朗日系数累乘值
		参数说明：
			atrs:vector<string>*  属性集合
			p：BIGNUM * mod p
		返回值:vector<ATRPATHLCM>*,匹配属性对应拉格朗日系数累乘值
		************************************************************************/	
		vector<ATRPATHLCM>* GetATRPATHLCM(vector<string>* atrs,BIGNUM *p);
		
		
		string GetAccessTree();
		
};

#endif
