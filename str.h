#ifndef HEAD_STR
#define HEAD_STR
#include <iostream> 
#include <boost/regex.hpp> 
#include <vector>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <stack>
#include <queue>
#include <openssl/sha.h>
#include "print.h"
using namespace std;

//整数转化为字符串
string int2str(int value);

int str2int(string value);
//按照子串sp分割**注意元字符（.|+[]()等）=_=采用boost正则表达式实现的
vector<string> splite(string str,string sp);

//去掉字符串中的空白符
string del_blank(string str);

//将字符串str中的sub子串替代为rep **注意元字符（.|+[]()等）=_=采用boost正则表达式实现的
string replace(string str,string sub,string rep);

//判断set是否包含value
bool contain(vector<string> *set,string value);

//将整数转化成字符串radix表示进制
char *itoa(int value,char *str,int radix);

//将字符串转化为二进制字符串 比如“12”->“11000100110010”
char * str2bin(unsigned char  *str,int len);

int unsignchar2hex(unsigned char*p,int len,char *r);

//SHA1-160
int hash(char *p,char*r);

#endif
