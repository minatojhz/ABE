#ifndef HEAD_POLYNOMIAL
#define HEAD_POLYNOMIAL
#include "tate.h"
#include "str.h"
#include "xml.h"
class Polynomial
{
	private:
		//data
		vector<BIGNUM *> *coefficients;//多项式系数
		BIGNUM *p;//mod p
	public:
		
		/**********************************************************************
		函数名称：Polynomial
		函数功能：构造函数,随机生成常数项为constant，最高项系数为len-1的多项式
		参数说明：
			constant:BIGNUM*,多项式常数项
			p:BIGNUM*,mod p
			len:int,多项式最高项系数为len-1
		************************************************************************/
		Polynomial(BIGNUM *constant,BIGNUM *p,int len);
		
		
		/**********************************************************************
		函数名称：Polynomial
		函数功能：构造函数,指定各项系数
		参数说明：
			coefficients:vector<BIGNUM *> *,各项系数
			p:BIGNUM*,mod p
		************************************************************************/
		Polynomial(vector<BIGNUM *> *coefficients,BIGNUM *p);
		Polynomial();
		Polynomial(const Polynomial &PL);
		
		void SetCoefficients(vector<BIGNUM *> *coefficients);
		void SetP(BIGNUM *p);

		vector<BIGNUM*>  GetCoefficients();
		BIGNUM * GetP();
		
		/**********************************************************************
		函数名称：GetFunctionValue
		函数功能：计算f(x) mod p 值
		参数说明：
			x:BIGNUM *,代入值
		返回值:BIGNUM*,f(x) mod p
		************************************************************************/
		BIGNUM *GetFunctionValue(BIGNUM *x);
		
		/**********************************************************************
		函数名称：GetLagIntCoe
		函数功能：计算拉格朗日插值系数
		参数说明：
			allParties: vector<int> *,参与方集合
			index: int 需计算的参与者
		返回值: BIGNUM*,参与者index的拉格朗日系数
		************************************************************************/
		BIGNUM *GetLagIntCoe(vector<int> *allParties,int index);
		
		Polynomial & operator= (const Polynomial &PL);
		friend 	ostream& operator<<(ostream &os,Polynomial &PL);
		
		~Polynomial();
		
};

#endif
