#include "Node.h"
Node::Node(TiXmlElement *element)
{
	const char * typeStr    = element->Value();
   	TiXmlAttribute *posAtr = element->FirstAttribute();
   	const char *    posAtrValue = posAtr->Value();
   	TiXmlAttribute *otherAtr = posAtr->Next();
   	const char *    otherAtrValue = otherAtr->Value();

   	isLeaf = strcmp(typeStr,"leaf") ? false : true;
   	position  = atoi(posAtrValue);
   	value = otherAtrValue;	
}

Node::Node(bool isLeaf ,int position,string value)
{
	SetAllValue(isLeaf ,position,value);
}

Node::Node()
{

}

void Node::SetPosition(int position)
{
	this->position = position;
}

void Node::SetValue(string value)
{
	this->value = value;
}

void Node::SetType(bool isLeaf)
{
	this->isLeaf = isLeaf ;
}

void Node::SetAllValue(bool isLeaf,int position,string value)
{
	this->isLeaf = isLeaf;
	this->position = position;
	this->value	= value;
}

void Node::GetAllValue(bool &isLeaf,int &position,string &value)
{
	isLeaf = this->isLeaf;
	position = this->position;
	value = this->value;
}
		
TiXmlElement * Node::ToTiXmlElement()
{
    string name = isLeaf ? "leaf":"node";
    string valueType  = isLeaf?"value":"threshold";
 
    string posAtrValue = int2str(position);
    TiXmlElement *element = new TiXmlElement("");
    element->SetValue(name.c_str());
    element->SetAttribute("position",posAtrValue.c_str());
    element->SetAttribute(valueType.c_str(),value.c_str());
    return element;
}

bool Node::IsLeaf()
{
    return isLeaf; 	
}

int Node::GetPosition()
{
	return position;
}

string Node::GetValue()
{
	return value;
}

ostream& operator<<(ostream &os,Node &node)
{
	TiXmlDocument *doc = element_wrap(node.ToTiXmlElement());
	string  str = xml2string(doc);
	os<<str;
	return os; 
}

Node & Node::operator= (const Node &node)
{
	this->isLeaf = node.isLeaf;
  	this->position = node.position;
   	this->value = node.value;
   	return *this;
}
