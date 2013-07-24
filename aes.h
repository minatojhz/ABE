#ifndef HEAD_AESFILE
#define HEAD_AESFILE

#include <openssl/evp.h>
#include <stdio.h>

#define N 1024
//Aes算法ecb模式加密文件
/**********************************************************************
函数名称：aes_enc_file 
函数功能：加密文件
处理过程：
	1.根据选择的密码算法以及口令，生成key和iv。
	2.把文件头写入密文文件
	3.循环读取原文文件数据加密后保存到密文文件路径中。
参数说明：
	szSrc: char *，待加密的原文文件路径
	szTarget: char *，加密后的密文文件保存路径
	key: unsigned char *, 口令
	iType: int, 加密类型（128或256）
	返回值：int 加密成功返回0,szSrc文件不存在返回-1,szTarget文件不存在返回-2,类型错误返回-3,加密失败返回-4
************************************************************************/
int aes_enc_file (char * szSrc,
					 char * szTarget ,
					 unsigned char * key,
					 int iType);

//Aes算法ecb模式解密文件
/**********************************************************************
函数名称：aes_dec_file
函数功能：解密文件
处理过程：
	1.根据选择的密码算法以及口令，生成key和iv。
	2.把文件头写入密文文件
	3.循环读取原文文件数据加密后保存到密文文件路径中。
参数说明：
	szSrc: char *，待解密的密文文件路径
	szTarget: char *，解密后的解密文件保存路径
	key: unsigned char *, 口令
	iType: int, 加密类型（128或256）
	返回值：int 解密成功返回0,szSrc文件不存在返回-1,szTarget文件不存在返回-2,类型错误返回-3,解密失败返回-4
************************************************************************/
int aes_dec_file (char * szSrc,
					 char * szTarget ,
					 unsigned char * key,
					 int iType);

#endif
