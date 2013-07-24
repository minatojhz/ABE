#ifndef HEAD_PRINT
#define HEAD_PRINT

#include <vector>
#include <stack>
#include <deque>
using namespace std;
template <typename T>
void vector_print(vector<T> &v)
{
	typename vector<T>::iterator iter;
    for (iter=v.begin();iter!=v.end();iter++)
    {
    	cout<<*iter<<endl;
    } 
}

template <typename T>
void stack_print(stack<T> &s)
{
	for(int i=0;i<s->size();i++)
	{
		cout<<*(&s.top()-i)<<endl;
	}
}
template <typename T>
void deque_print(deque<T> &d)
{
	typename deque<T>::iterator iter; 
    for(iter=d.begin();iter!=d.end();iter++)
    { 
    	cout<<*iter<<endl;
    } 
}
#endif
