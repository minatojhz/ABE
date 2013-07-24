#ifndef HEAD_NODE
#define HEAD_NODE
#include "tate.h"
#include "str.h"
#include "xml.h"
class Node
{
 	private :
		bool isLeaf;
  	 	int position;
   	 	string value;
	public:
		//结构化xml构造Node
		Node(TiXmlElement *element);
        Node();
		Node(bool isLeaf ,int position,string value);

    	void SetAllValue(bool isLeaf ,int position,string value);
	   	void GetAllValue(bool &isLeaf,int &position,string &value);
		int GetPosition();
		string GetValue();
		
		bool IsLeaf();
		void SetPosition(int position);
		void SetValue(string value);
		void SetType(bool isLeaf);
		//转化结构化xml
		TiXmlElement * ToTiXmlElement();
		
		Node & operator= (const Node &node);
		friend 	ostream& operator<<(ostream &os,Node &node);
		
};

#endif
