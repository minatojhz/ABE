#include"aes.h"

int aes_enc_file (char * szSrc,
					 char * szTarget ,
					 unsigned char * key,
					 int iType)
{
	unsigned char ukey[EVP_MAX_KEY_LENGTH];
	unsigned char iv[EVP_MAX_IV_LENGTH];
	unsigned char in[N];
	int inl;   //输入数据大小
	unsigned char out[N];
	int outl;   //输出数据大小

	int isSuccess;

	FILE *fpIn;
	FILE *fpOut;

	EVP_CIPHER_CTX ctx;   //evp加密上下文环境
	const	EVP_CIPHER *cipher;

	fpIn = fopen(szSrc,"rb");
	if(fpIn==NULL)
	{
		return -1;
	}
	fpOut = fopen(szTarget,"w+");
	if(fpOut==NULL)
	{
		fclose(fpIn);
		return -2;
	}

	//选择算法
	if(iType == 128)
	{
		cipher = EVP_aes_128_ecb();
	}
	else if(iType == 256)
	{
		cipher = EVP_aes_256_ecb();
	}
	else
	{
		fclose(fpIn);
		fclose(fpOut);
		return -3;
	}
	//生成ukey和iv
	int len = sizeof(key);
	EVP_BytesToKey(cipher,EVP_md5(),NULL,(const unsigned char *)key,len-1,1,ukey,iv);

	//初始化ctx，加密算法初始化
	EVP_CIPHER_CTX_init(&ctx);
	isSuccess = EVP_EncryptInit_ex(&ctx,cipher,NULL,ukey,iv);
	if(!isSuccess)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		fclose(fpIn);
		fclose(fpOut);
		return -4;
	}

	//加密文件
	for(;;)
	{
		inl = fread(in,1,N,fpIn);
		if(inl<=0)
			break;

		isSuccess = EVP_EncryptUpdate(&ctx,out,&outl,in,inl);
		if(!isSuccess)
		{
			EVP_CIPHER_CTX_cleanup(&ctx);
			fclose(fpIn);
			fclose(fpOut);
			return -4;
		}
		fwrite(out,1,outl,fpOut);
	}
	isSuccess = EVP_EncryptFinal_ex(&ctx,out,&outl);
	if(!isSuccess)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		fclose(fpIn);
		fclose(fpOut);
		return -4;
	}
		
	fwrite(out,1,outl,fpOut);

	EVP_CIPHER_CTX_cleanup(&ctx);
	fclose(fpIn);
	fclose(fpOut);
	return 0;
}

int aes_dec_file (char * szSrc,
					 char * szTarget ,
					 unsigned char * key,
					 int iType)
{
	unsigned char ukey[EVP_MAX_KEY_LENGTH];
	unsigned char iv[EVP_MAX_IV_LENGTH];
	unsigned char in[N];
	int inl;   //输入数据大小
	unsigned char out[N];
	int outl;   //输出数据大小

	int isSuccess;

	FILE *fpIn;
	FILE *fpOut;

	EVP_CIPHER_CTX ctx;   //evp加密上下文环境
	const EVP_CIPHER *cipher;

	fpIn = fopen(szSrc,"rb");
	if(fpIn==NULL)
	{
		return -1;
	}
	fpOut = fopen(szTarget,"w+");
	if(fpOut==NULL)
	{
		fclose(fpIn);
		return -2;
	}

	//选择算法
	if(iType == 128)
	{
		cipher = EVP_aes_128_ecb();
	}
	else if(iType == 256)
	{
		cipher = EVP_aes_256_ecb();
	}
	else
	{
		fclose(fpIn);
		fclose(fpOut);
		return -3;
	}
	//生成ukey和iv
	int len = sizeof(key);
	EVP_BytesToKey(cipher,EVP_md5(),NULL,(const unsigned char *)key,len-1,1,ukey,iv);

	//初始化ctx，加密算法初始化
	EVP_CIPHER_CTX_init(&ctx);
	isSuccess = EVP_DecryptInit_ex(&ctx,cipher,NULL,ukey,iv);
	if(!isSuccess)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		fclose(fpIn);
		fclose(fpOut);
		return -4;
	}

	//加密文件
	for(;;)
	{
		inl = fread(in,1,N,fpIn);
		if(inl<=0)
			break;

		isSuccess = EVP_DecryptUpdate(&ctx,out,&outl,in,inl);
		if(!isSuccess)
		{
			EVP_CIPHER_CTX_cleanup(&ctx);
			fclose(fpIn);
			fclose(fpOut);
			return -4;
		}
		fwrite(out,1,outl,fpOut);
	}
	isSuccess = EVP_DecryptFinal_ex(&ctx,out,&outl);
	if(!isSuccess)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		fclose(fpIn);
		fclose(fpOut);
		return -4;
	}
		
	fwrite(out,1,outl,fpOut);
	EVP_CIPHER_CTX_cleanup(&ctx);
	fclose(fpIn);
	fclose(fpOut);
	return 0;
}

