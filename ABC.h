#ifndef HEAD_ABC
#define HEAD_ABC

#include "ATRGROUP.h"
#include "ABCSK.h"
#include "ABCPK.h"
#include "ABCMK.h"
#include "ABCEBLOCK.h"
#include "AccessTree.h"
#include "aes.h"
#include "RuleBuilder.h"

typedef struct
{
	string name;	
    BIGNUM * value;
}LEAFSTORE;

class ABC
{
	private :
		//data
		ABCMK mk;//主密钥
		ABCPK pk;//公钥
		ABCSK sk;//私钥
		EC_GROUP *group;//椭圆曲线群


		//function
	
		void InitGroup(EC_GROUP *group);


		/**********************************************************************
		函数名称：Init
		函数功能：初始化 ABC 主密钥
		参数说明：
			a0: BIGNUM *，初始化ABC主密钥的一个因子
			a1: BIGNUM *，初始化ABC主密钥的一个因子
			group: EC_GROUP *,椭圆曲线群
		返回值：void
		************************************************************************/
		void Init(BIGNUM *a0,BIGNUM *a1,EC_GROUP *group);


		/**********************************************************************
		函数名称：ContainSet
		函数功能：判断私钥sk中含有的属性值的集合是否包含 atrs属性集合
		参数说明：
			atrs:vector<string>*，属性集合
		返回值：包含返回true，否则返回false	
		************************************************************************/
		bool ContainSet(vector<string> *atrs);


		LEAFSTORE InitLeafStore(string name ,BIGNUM *s);


		/**********************************************************************
		函数名称：GenerateRandomLeaf
		函数功能：加密过程生成叶子节点的随机值
		参数说明：
			root:TiXmlElement *，策略树
			s:BIGNUM *,树根的初始值
			store:vector<LEAFSTORE> *,随机值的存储位置
		返回值：包含返回true，否则返回false	
		************************************************************************/
		void GenerateRandomLeaf(TiXmlElement *policy,BIGNUM * s,vector<LEAFSTORE> *store);


		/**********************************************************************
		函数名称：String2Point
		函数功能：字符串转化成椭圆曲线上的点
		参数说明：
			str:char *
		返回值：EC_POINT＊	
		************************************************************************/
		EC_POINT* String2Point(char*str);
		string Integer2String(int value,int len);
		void HandleAtrs(vector<string> *atrs);
		
	public :
		/**********************************************************************
		函数名称：ABC
		函数功能：ABC构造函数　指定主密钥的生成因子
		参数说明：
			a0: BIGNUM *，初始化ABC主密钥的一个因子
			a1: BIGNUM *，初始化ABC主密钥的一个因子
			group: EC_GROUP *,椭圆曲线群
		************************************************************************/
		ABC(BIGNUM *a0,BIGNUM *a1,EC_GROUP *group);


		/**********************************************************************
		函数名称：ABC
		函数功能：ABC构造函数　随机生成主密钥的生成因子
		参数说明：
			group: EC_GROUP *,椭圆曲线群
		************************************************************************/		
		ABC(EC_GROUP *group);


		ABC();


		/**********************************************************************
		函数名称：GenerateABCSK
		函数功能：ABC 生成私钥(mk主密钥需要赋值)
		参数说明：
			atrs: vector<string> *,属性结合
		返回值：属性结合对应的私钥sk
		************************************************************************/
		ABCSK GenerateABCSK(vector<string> *atrs);


		/**********************************************************************
		函数名称：Encrypt
		函数功能：ABC 加密(pk公钥需要赋值)
		参数说明：
			m: BIGNUM *,明文
			policy: TiXmlElement *,加密策略
		返回值：返回密文块ABCEBLOCK
		************************************************************************/
		ABCEBLOCK Encrypt(BIGNUM *m,TiXmlElement *policy);


		/**********************************************************************
		函数名称：Decrypt
		函数功能：ABC 解密(sk私钥需要赋值)
		参数说明：
			eblock: ABCEBLOCK ,加密块
			policy: TiXmlElement *,加密策略
		返回值：返回明文BIGNUM *
		************************************************************************/		
		BIGNUM * Decrypt(ABCEBLOCK &eblock);


		/**********************************************************************
		函数名称：Delegate
		函数功能：ABC 授权(sk私钥和pk公钥需要赋值)
		参数说明：
			atrs:vector<string> *,属性集合(sk私钥属性集合包含atrs)
		返回值：返回授权私钥ABCSK
		************************************************************************/
		ABCSK Delegate(vector<string> *atrs);

				
		/**********************************************************************
		函数名称：EncryptFile
		函数功能：ABC 加密文件(ABC和AES-128结合数字信封加密文件pk公钥需要赋值)
		参数说明：
			szSrc:char *,明文文件路径
			szTarget:char*,加密文件存储路径
			eblock:ABCEBLOCK,数字信封头的加密块存储对象
			policy:TiXmlElement *,加密策略
		返回值：int 加密成功返回0,szSrc文件不存在返回-1,szTarget文件不存在返回-2,加密失败返回-4
		************************************************************************/
		int EncryptFile(char *szSrc,char *szTarget,ABCEBLOCK &eblock ,TiXmlElement *policy);

		/**********************************************************************
		函数名称：DecryptFile
		函数功能：ABC 解密文件(sk私钥需要赋值)
		参数说明：
			szSrc:char *,加密文件路径
			szTarget:char*,解密文件存储路径
			eblock:ABCEBLOCK,数字信封头的加密块
			policy:TiXmlElement *,加密策略
		返回值：int 解密成功返回0,szSrc文件不存在返回-1,szTarget文件不存在返回-2,解密失败返回-4
		************************************************************************/
		int DecryptFile(char *szSrc,char *szTarget,ABCEBLOCK &eblock);

		ABCMK GetMK();
		ABCSK GetSK();
		ABCPK GetPK();

		void SetMK(ABCMK &mk);
		void SetPK(ABCPK &pk);
		void SetSK(ABCSK &sk);

		~ABC();
				
};
#endif 
