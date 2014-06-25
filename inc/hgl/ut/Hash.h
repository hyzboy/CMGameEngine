#ifndef HGL_UTIL_HASH_INCLUDE
#define HGL_UTIL_HASH_INCLUDE

#include<hgl/type/BaseString.h>
namespace hgl
{
	namespace util
	{
		/**
		 * Hash算法枚举
		 */
		enum HASH_ALGORITHML			///Hash算法枚举
		{
			hashNone=0,

			hashMD5,					///<最常用的HASH算法
			hashMD4,					///<较MD5在安全性下稍弱。但仍未被攻破，且较MD5更快
			hashSHA1,					///<较MD5更为安全，但计算较慢

			hashEnd
		};//enum HASH_ALGORITHML

		/**
		 * Hash编码结构模板
		 */
		template<int SIZE> struct HashCode
		{
			unsigned char code[SIZE];

		public:

			HashCode()
			{
				hgl_zero(code);
			}

			static const int size(){return SIZE;}

			const unsigned char operator[](int index)const
			{
				return code[index];
			}

			const int CompFunc(const HashCode<SIZE> &hash)const
			{
				const unsigned char *s=code;
				const unsigned char *t=hash.code;

				for(int i=0;i<SIZE;i++)
				{
					if(*s!=*t)
						return(*s-*t);

					s++;
					t++;
				}

				return(0);
			}

			CompOperator(const HashCode<SIZE> &,CompFunc)
		};//template<int SIZE> struct HashCode

		typedef HashCode<16> MD5Code;
		typedef HashCode<16> MD4Code;
		typedef HashCode<20> SHA1Code;

		/**
		 * 散列值计算功能基类
		 */
		class Hash											///散列值计算功能基类
		{
		public:

			virtual ~Hash()HGL_DEFAULT_MEMFUNC;

			virtual const int GetHashBytes()const=0;		///<取得HASH码字节长度(MD4/MD5为16,SHA1为20)

			virtual void Init()=0;							///<初始化散列值计算
			virtual void Update(const void *,int)=0;		///<提交新的数据
			virtual void GetResult(void *)=0;				///<结束并取得结果
		};//class Hash

		Hash *CreateHash(HASH_ALGORITHML);					///<创建一个hash值计算类实例

		/**
		* 计算一段数据的Hash值
		* @param data 数据指针
		* @param size 数据长度
		* @param hash_code 计算后的hash值存放处
		* @param ha hash算法
		* @return 是否计算成功
		*/
		bool CountHash(const void *data,int size,void *hash_code,HASH_ALGORITHML ha);

		bool CountMD5(const void *data,int size,MD5Code &md5){return CountHash(data,size,&md5,hashMD5);}
		bool CountMD4(const void *data,int size,MD4Code &md4){return CountHash(data,size,&md4,hashMD4);}
		bool CountSHA1(const void *data,int size,SHA1Code &sha1){return CountHash(data,size,&sha1,hashSHA1);}

		/**
		* 取得一个文件的hash值
		* @param filename 文件名
		* @param hash_code 计算后的hash存放处
		* @param ha hash算法
		* @return 是否计算成功
		*/
		bool GetFileHash(const OSString &,void *hash_code,HASH_ALGORITHML ha);
	}//namespace util
}//namespace hgl
#endif//HGL_UTIL_HASH_INCLUDE
