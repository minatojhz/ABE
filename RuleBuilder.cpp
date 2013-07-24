#include "RuleBuilder.h"
RuleBuilder::RuleBuilder(string ruleString)
{
	this->ruleString = ruleString;
}
TiXmlElement * RuleBuilder::RuleString2XMLElement()
{

   	TiXmlElement *root = new TiXmlElement("");
   	string express = del_blank(ruleString);
   	ConstructOr(express,root,1);
   	return root;
}
string RuleBuilder::RuleString2XMLString()
{
    TiXmlElement * root = RuleString2XMLElement();
    TiXmlDocument * doc = element_wrap(root);
    return xml2string(doc);
}
int RuleBuilder::SaveFile(char *szTarget)
{
	FILE *fpOut = fopen(szTarget,"w+");
	if(fpOut==NULL)
	{
		return 0;
	}
   	TiXmlElement * root = RuleString2XMLElement();
    TiXmlDocument * doc = element_wrap(root);
    doc->SaveFile(szTarget);
    return 1;
}
bool  RuleBuilder::CheckRule(string rule)
{
	string data = "(\\^|([^&\\|,\\(\\)=><:]+(:[0-9]+(=|>=|>|<=|<)[0-9]+)?))";
	string oper = "(\\|\\||&&)";
	string item = "("+data+oper+")*"+data;
	string syntax  = string("([0-9]+,")+"("+item+",)*)?"+item;
	boost::regex reg(syntax);
	
   	string express = del_blank(rule);
   	
   	vector<string> vectorReplacement = ReplaceBracket(express);

   	if(!boost::regex_match(vectorReplacement[0],reg))
   	{
   		cout<<vectorReplacement[0]<<endl;
       	return false;
   	}

   	for(int i = 1;i<vectorReplacement.size();i++)
   	{
    	if(!CheckRule(vectorReplacement[i]))
      	{
            return false;
      	}
   	}
    
   	return true;
}

bool RuleBuilder::IsThreshold(string rule)
{
	vector<string> vectorReplacement = ReplaceBracket(rule);
	boost::regex reg("(.+,)+.+");
	if(boost::regex_match(vectorReplacement[0],reg))
	{
		return true;
	}
	
	return false;
	
}

int TranslateOperator(string str)
{
	string OPERATOR[]={"=",">=",">","<=","<"};
	for(int i=0;i<5;i++)
	{
		if(OPERATOR[i] == str)
		{
			return i+1;
		}
	}
	
	return -1;
}
bool RuleBuilder::Check()
{
   	return CheckRule(ruleString);
}
vector<string> RuleBuilder::ReplaceBracket(string express)
{
    string regString = "\\((?:(?>[^()]+)|(?0))*\\)"; 

    boost::smatch what; 
    boost::regex  reg(regString); 

    string::const_iterator start = express.begin(); 
    string::const_iterator end = express.end();
 
	vector<string> vectorReplacement = vector<string>();

    vectorReplacement.push_back(boost::regex_replace(express,reg,"^"));


   	while (boost::regex_search(start,end,what,reg)) 
    { 
    	string msg(what[0].first+1, what[0].second-1); 
	    vectorReplacement.push_back(msg);  
        start = what[0].second;
    }

	return vectorReplacement;
} 

string RuleBuilder::RecoverBracket(string express,vector<string> vec,int &count)
{	
	string::size_type pos = 0;

    while((pos=express.find_first_of("^",pos))
                    != string::npos)
   	{
    	express = express.replace(pos,1,"("+vec[count++]+")");
   	}
    return express;
}
void RuleBuilder::ConstructOr(string express,TiXmlElement * parent,int index)
{
    vector<string> vectorReplacement = ReplaceBracket(express);

    vector<string> vectorOr = splite(vectorReplacement[0],"\\|\\|");

    int count = 1;

    if(vectorOr.size() > 1)
    {
    	//或门操作
        init_node(parent,1,index,"1");

        for(int i=0;i<vectorOr.size();i++)
        {
        	TiXmlElement *child = new TiXmlElement("");
            parent->LinkEndChild(child);

            if(vectorOr[i] == SP)
            {
                ConstructOr(vectorReplacement[count++],child,i+1);
            }
            else
            {
                string undoExpress = RecoverBracket(vectorOr[i],vectorReplacement,count);
                ConstructAnd(undoExpress,child,i+1);
            }
        }
    }
    else
    {
        string undoExpress = RecoverBracket(vectorOr[0],vectorReplacement,count);
        ConstructAnd(undoExpress,parent,index);  
    }
}

void RuleBuilder::ConstructAnd(string express,TiXmlElement *parent,int index)
{
    vector<string> vectorReplacement = ReplaceBracket(express);
    vector<string> vectorAnd = splite(vectorReplacement[0],"&&");

    int count =1;

    if(vectorAnd.size() > 1)
    {
        //与门操作
        init_node(parent,1,index,int2str(vectorAnd.size()));

        for(int i=0;i<vectorAnd.size();i++)
        {
            TiXmlElement * child = new TiXmlElement("");
            parent->LinkEndChild(child);
       
            if(vectorAnd[i] == SP)
            {
            	if(IsThreshold(vectorReplacement[count]))
            	{
            		ConstructThreshold(vectorReplacement[count++],child,i+1);
            	}
            	else
            	{
                	ConstructOr(vectorReplacement[count++],child,i+1);
                }
            }   
            else
            {
                OperateRule(vectorAnd[i],child,i+1);
            }
        }
    }
    else
    {   
    	 if(vectorAnd[0] == SP)
         {
            if(IsThreshold(vectorReplacement[1]))
            {
            	ConstructThreshold(vectorReplacement[1],parent,index);
            }
            else
            {
                ConstructOr(vectorReplacement[1],parent,index);
            }
         }   
         else
         {
               OperateRule(vectorAnd[0],parent,index);
         }
    }

}
void RuleBuilder::Operate(int type,int value,string atr,int len,TiXmlElement *parent,int index)
{
	string buffer = Integer2String(value,len);
	
	for(int i=0;i<buffer.size();i++)
	{
		if(IsNode(type,buffer,i))
		{
			int pos = i? 2:index;
			string threshold = IsAnd(type,i,buffer)?"2":"1";
			init_node(parent,1,pos,threshold);
				
			TiXmlElement *left  = new TiXmlElement("");
			TiXmlElement *right = new TiXmlElement("");
			parent->LinkEndChild(left);
			parent->LinkEndChild(right);
			
			init_node(left,0,1,GetLeftValue(type,atr,buffer,i));
			parent = right;
		}
		else
		{
			init_node(parent,0,2,GetLeftValue(type,atr,buffer,i));
			break;
		}
	}
	
	
	

}

bool RuleBuilder::IsAnd(int type,int index,string value)
{
	switch(type)
	{
		case EQ:
			return true;
		case GE:
		case GT:
			if(value[index] == '1')
				return true;
			else
				return false;
		case LE:
		case LT:
			if(value[index] == '0')
				return true;
			else
				return false;
		default:
			return false;
	}
	

}

string RuleBuilder::Integer2String(int value,int len)
{
	char buf[30];
	itoa(value,buf,2);
	string valueString(buf);
	while(valueString.size()<len)
		valueString = "0" + valueString;
	return valueString;
}
bool RuleBuilder::IsNode(int type ,string value,int index)
{
	int one  = value.rfind("1");
	int zero = value.rfind("0");
	
	switch(type)
	{
		case EQ:
			if(index<value.size()-1)
				return true;
			else
				return false;
			break;
		case GE:
		case LT:
			if(index<one)
				return true;
			else
				return false;
		case GT:
		case LE:
			if(index < zero)
				return true;
			else
				return false;
		default:
			return false;
	}

}

string RuleBuilder::GetLeftValue(int type,string atr,string value,int index)
{
	// value to string
	char bit;
	string initValue(value.size(),'*');		
	switch(type)
	{
		case EQ:
			bit = value[index];
			break;
		case GE:
		case GT:
			bit = '1';
			break;
		case LE:
		case LT:
			bit = '0';
			break; 
	}
	initValue.replace(index,1,1,bit);
	initValue = atr +":"+ initValue;	
	return initValue;
}

void RuleBuilder::OperateRule(string rule,TiXmlElement *parent,int index)
{
	boost::regex expression("(.+):([0-9]+)(>=|<=|=|>|<)(.+)");
	boost::smatch what;
	if(!boost::regex_search(rule, what, expression))
	{
		init_node(parent,0,index,rule);
	}
	else
	{   
		Operate(TranslateOperator(what[3]),
				str2int(what[4]),
				what[1],
				str2int(what[2]),
				parent,
				index);	
	}

}


void RuleBuilder::ConstructThreshold(string express,TiXmlElement *parent,int index)
{
	vector<string> vectorReplacement = ReplaceBracket(express);
    vector<string> vectorAnd = splite(vectorReplacement[0],",");
    int count =1;
    
    if(vectorAnd.size()>1)
    {
    	init_node(parent,1,index,vectorAnd[0]);
    	
    	for(int i=1;i<vectorAnd.size();i++)
    	{
    		TiXmlElement * child = new TiXmlElement("");
            parent->LinkEndChild(child);
            string recover = RecoverBracket(vectorAnd[i],vectorReplacement,count);
            ConstructOr(recover,child,i);
    	}
    
    }


}

ostream& operator<<(ostream &os,RuleBuilder &ruleBuilder)
{
	string  str = ruleBuilder.RuleString2XMLString();
	os<<str;
	return os; 
}
