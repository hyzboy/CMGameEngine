#include<hgl/type/DataType.h>

namespace hgl
{
	/**
	* 溢出加密
	* @param target 加密后的数据存放区
	* @param source 加密前的数据存放区
	* @param size 数据长度
	* @param key 密码
	* @param key_size 密码长度
	*/
	void OverflowEncrypt(void *target,void *source,int size,void *key,int key_size)
	{
		int n;
		uint8 *key_p;
		uint8 *tp,*sp;

		n=key_size;
		key_p=(uint8 *)key;

		tp=(uint8 *)target;
		sp=(uint8 *)source;

		while(size--)
		{
			uint tmp=(*sp)+(*key_p);

			if(tmp>0xFF)
				*tp=tmp-0x100;
			else
				*tp=tmp;

        	*key_p^=*tp;

			tp++;
			sp++;

			if(--n==0)
			{
				n=key_size;
				key_p=(uint8 *)key;
			}
			else
				key_p++;
		}
	}

	/**
	* 溢出解密
	* @param target 解密后的数据存放区
	* @param source 解密前的数据存放区
	* @param size 数据长度
	* @param key 密码
	* @param keysize 密码长度
	*/
	void OverflowDecrypt(void *target,void *source,int size,void *key,int key_size)
	{
		int n;
		uint8 *key_p;
		uint8 *tp,*sp;

		n=key_size;
		key_p=(uint8 *)key;

		tp=(uint8 *)target;
		sp=(uint8 *)source;

		while(size--)
		{
			if(*key_p>*sp)
				*tp=*sp+0x100-(*key_p);
			else
				*tp=*sp-(*key_p);

        	*key_p^=*sp;

			tp++;
			sp++;

			if(--n==0)
			{
				n=key_size;
				key_p=(uint8 *)key;
			}
			else
				key_p++;
		}
	}
}
