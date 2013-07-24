#include "str.h"

string int2str(int value)
{
    char str[20];
    sprintf(str,"%d",value);
    return str;
}
int str2int(string value)
{ 
	return atoi(value.c_str());
}


char *itoa(int value,char *str,int radix)  
{   
    char tmp[32]={'\0'};  
    int i,j;  
    for(i=0;i<32 && value;i++)  
    {                 
    	tmp[i]=(value%radix)+'0';  
        value=value/radix;
    }   
    for(j=0,i--;i>=0;i--)
    	str[j++]=tmp[i];
    str[j]='\0';  
    return str;  
}  

char * str2bin(unsigned char *str,int len)
{
	bool flag = false;
	char buf[9];
	char *store = new char(len*8);
	store[0] = '\0';
	
	for(int i=0;i<len;i++)
	{
		itoa(str[i],buf,2);
		if(flag)
		{
			for(int j=0;j<8-strlen(buf);j++)
				strcat(store,"0");
		}
		else
		{
			if(str[i] == '0')
				continue;
			flag= true;
		}
		strcat(store,buf);
	}
	if(!flag)
	{
		strcat(store,"0");
	}
	
	return store;
}

vector<string> splite(string str,string sp)
{
    boost::regex reg(sp);
    boost::sregex_token_iterator it(str.begin(),str.end(),reg,-1);
    boost::sregex_token_iterator end;

    vector<string> spv= vector<string>();
    while(it != end)
    {
        spv.push_back(*it++);
    }

    return spv;
}

string del_blank(string str)
{
    return replace(str,"\\s","");
}

string replace(string str,string sub,string rep)
{
    boost::regex reg(sub);
    return boost::regex_replace(str,reg,rep);
}

bool contain(vector<string> *set,string value)
{
    vector<string>::iterator result = find(set->begin( ),set->end( ),value);
    return result != set->end();
}

int unsignchar2hex(unsigned char*p,int len,char *r)
{
	int i = 0;
   	for(;i<len;i++)
   	{
    	int value = *(p+i);
   	   	int  vl = value & 0x0F;
       	int  vh = (value &0xF0)>>4;
       	*(r+2*i +1) = vl < 10 ? vl +'0':vl - 10 + 'A';
       	*(r+2*i) = vh < 10 ? vh +'0':vh - 10 + 'A';
   	}

   	*(r+2*i)='\0';
   	
   	return 0;

}

int hash(char *str,char *r)
{
   unsigned char store[21];
   SHA1((unsigned char*)str,strlen(str),store);
    
   unsignchar2hex(store,20,r);
   
   return 0;

}
