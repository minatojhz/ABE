#ifndef HEAD_ATRPATHLCM
#define HEAD_ATRPATHLCM
#include "tate.h"
#include "str.h"
#include "xml.h"

class ATRPATHLCM
{
	private:
		//data	
    	string name;//属性值
    	BIGNUM *lcm;//拉格朗日系数累乘值
		BIGNUM *p;//素数域
    	int track;//跟踪父亲节点在栈中位置
    	
	public:
		ATRPATHLCM(string name,BIGNUM *lcm,BIGNUM *p,int track);
		ATRPATHLCM(string name,BIGNUM *p);
		ATRPATHLCM();
		ATRPATHLCM(const ATRPATHLCM &AM);

		void SetName(string name);
		void SetLCM(BIGNUM *lcm);
		void SetP(BIGNUM *p);
		void SetTrack(int track);		
		
		string GetName();
		BIGNUM * GetLCM();
		BIGNUM * GetP();
		int GetTrack();
		
		/**********************************************************************
		函数名称：MulLagCoe
		函数功能：拉格朗日系数累乘
		参数说明：
			lc: BIGNUM *,累乘值
		返回值:void
		************************************************************************/	
		void MulLagCoe(BIGNUM *lc);
		
		ATRPATHLCM & operator= (const ATRPATHLCM &AM);
		~ATRPATHLCM();
		bool IsNULL();
};


#endif
