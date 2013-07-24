#ifndef HEAD_RULEBUILDER
#define HEAD_RULEBUILDER
#include "tate.h"
#include "str.h"
#include "xml.h"
#define EQ 1
#define GE 2
#define GT 3
#define LE 4
#define LT 5
#define SP "^"
class RuleBuilder
{
	private:
		//data 
		//eg.(human||sale)&&(2,woman,age:8>25,professor)
		//支持或||,与&&,门限(num,...,...,)三种基础属性操作，属性分为字符串属性和数值属性
		//字符串属性直接写就可以，数值属性格式为:(name):(len)(operator)(value)
		//name为属性名 len属性数值表示位数 operator (>,>=,<,<=,=) value 整数值
		string ruleString;
		
		//function
		
		//检测规则
		bool CheckRule(string rule);
		//替换括号内容	
		vector<string> ReplaceBracket(string express);
		//恢复括号内容
		string RecoverBracket(string express,vector<string> vec,int &count);
		//或操作	
		void ConstructOr(string express,TiXmlElement * parent,int index);
		//与操作
		void ConstructAnd(string express,TiXmlElement *parent,int index);
		//门限操作
		void ConstructThreshold(string express,TiXmlElement *parent,int index);
		//判断是否是门限
       	bool IsThreshold(string rule);
       	//规则处理（>,>=,<,<=,=）
       	void OperateRule(string rule,TiXmlElement *element,int index);
       	//规则叶子值
       	string GetLeftValue(int type,string atr,string value,int index);
       	//判断处理规则位置的类型
       	bool IsNode(int type ,string value,int index);
       	//整数转化为二进制数
       	string Integer2String(int value,int len);
       	//规则处理的门限类型
       	bool IsAnd(int type,int index,string value);
       	//操作
       	void Operate(int type,int value,string atr,int len,TiXmlElement *parent,int index);
	public:
		//构造函数(departmant = human || department = sale) && age = 40
		RuleBuilder(string ruleString);
		//将规则字符串转化成xml
		TiXmlElement * RuleString2XMLElement();
        //规则字符串转化成xml字符串
        string RuleString2XMLString();
		//检查语法规则
		bool Check();
		//生成策略文件
       	int SaveFile(char *path);
		
       	friend ostream& operator<<(ostream &os,RuleBuilder &ruleBuilder);
};

#endif
