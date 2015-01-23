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

			hashAdler32,				///<一种较CRC32更为安全的快速HASH算法
			hashCRC32,					///<最快速的HASH算法，但最不安全，仅用于低安全性的简单验证
			hashMD4,					///<较MD5在安全性下稍弱。但仍未被攻破，且较MD5更快
			hashMD5,					///<最常用的HASH算法
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

		typedef HashCode<4> CRC32Code;
		typedef HashCode<4> Adler32Code;
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

			virtual void GetName(UTF8String &)const=0;		///<取得HASH算法的名称
			virtual void GetName(UTF16String &)const=0;		///<取得HASH算法的名称
			virtual const int GetHashBytes()const=0;		///<取得HASH码字节长度(MD4/MD5为16,SHA1为20)

			virtual void Init()=0;							///<初始化散列值计算
			virtual void Update(const void *,uint)=0;		///<提交新的数据
			virtual void Final(void *)=0;					///<结束并取得结果
		};//class Hash

		Hash *CreateHash(HASH_ALGORITHML);					///<创建一个hash值计算类实例

		/**
		* 计算一段数据的Hash值
		* @param data 数据指针
		* @param size 数据长度
		* @param ha hash算法
		* @param hash_code 计算后的hash值存放处
		* @return 是否计算成功
		*/
		bool CountHash(const void *data,int size,HASH_ALGORITHML ha,void *hash_code);

		inline bool CountAdler32(const void *data,int size,Adler32Code &hc){return CountHash(data,size,hashAdler32	,&hc);}
		inline bool CountCRC32	(const void *data,int size,CRC32Code &	hc){return CountHash(data,size,hashCRC32	,&hc);}
		inline bool CountMD4	(const void *data,int size,MD4Code &	hc){return CountHash(data,size,hashMD4		,&hc);}
		inline bool CountMD5	(const void *data,int size,MD5Code &	hc){return CountHash(data,size,hashMD5		,&hc);}
		inline bool CountSHA1	(const void *data,int size,SHA1Code &	hc){return CountHash(data,size,hashSHA1		,&hc);}
		
		/**
		* 取得一个文件的hash值
		* @param filename 文件名
		* @param ha hash算法
		* @param hash_code 计算后的hash存放处
		* @return 是否计算成功
		*/
		bool GetFileHash(const OSString &,HASH_ALGORITHML ha,void *hash_code);
	}//namespace util
}//namespace hgl
#endif//HGL_UTIL_HASH_INCLUDE
