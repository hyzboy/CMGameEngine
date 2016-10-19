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

		const int hash_code_bytes[]={0,4,4,16,16,20};		//hash码长度

		/**
		 * 散列值计算功能基类
		 */
		class Hash											///散列值计算功能基类
		{
		public:

			virtual ~Hash()=default;

			virtual void GetName(UTF8String &)const=0;		///<取得HASH算法的名称
			virtual void GetName(UTF16String &)const=0;		///<取得HASH算法的名称
			virtual const int GetHashBytes()const=0;		///<取得HASH码字节长度(MD4/MD5为16,SHA1为20)

			virtual void Init()=0;							///<初始化散列值计算
			virtual void Update(const void *,uint)=0;		///<提交新的数据
			virtual void Final(void *)=0;					///<结束并取得结果
		};//class Hash

		Hash *CreateAdler32Hash();
		Hash *CreateCRC32Hash();
		Hash *CreateMD4Hash();
		Hash *CreateMD5Hash();
		Hash *CreateSHA1Hash();

		template<HASH_ALGORITHML ha> Hash *CreateHash();					///<创建一个hash值计算类实例

		template<> inline Hash *CreateHash<hashAdler32	>(){return CreateAdler32Hash();}
		template<> inline Hash *CreateHash<hashCRC32	>(){return CreateCRC32Hash();}
		template<> inline Hash *CreateHash<hashMD4		>(){return CreateMD4Hash();}
		template<> inline Hash *CreateHash<hashMD5		>(){return CreateMD5Hash();}
		template<> inline Hash *CreateHash<hashSHA1		>(){return CreateSHA1Hash();}

		inline Hash *CreateHash(HASH_ALGORITHML ha)
		{
			if(ha<=hashNone||ha>=hashEnd)return(nullptr);

			using CreateHashFunc=Hash *(*)();

			const CreateHashFunc func[hashEnd-1]=
			{
				CreateAdler32Hash,
				CreateCRC32Hash,
				CreateMD4Hash,
				CreateMD5Hash,
				CreateSHA1Hash
			};

			return func[ha-1]();
		}

		/**
		* 计算一段数据的Hash值
		* @param data 数据指针
		* @param size 数据长度
		* @param ha hash算法
		* @param hash_code 计算后的hash值存放处
		* @return 是否计算成功
		*/
		template<HASH_ALGORITHML ha> bool CountHash(const void *data,int size,void *hash_code)
		{
			if(!data||size<=0||!hash_code)return(false);

			Hash *h=CreateHash<ha>();

			if(!h)return(false);

			h->Init();
			h->Update(data,size);
			h->Final(hash_code);

			delete h;
			return(true);
		}

		/**
		* 计算一段数据的Hash值
		* @param data 数据指针
		* @param size 数据长度
		* @param ha hash算法
		* @param hash_code 计算后的hash值存放处
		* @return 是否计算成功
		*/
		inline bool CountHash(const void *data,int size,HASH_ALGORITHML ha,void *hash_code)
		{
			if(!data||size<=0||ha<=hashNone||ha>=hashEnd||!hash_code)return(false);

			using CountHashFunc=bool (*)(const void *,int size,void *);

			const CountHashFunc func[hashEnd-1]=
			{
				CountHash<hashAdler32	>,
				CountHash<hashCRC32		>,
				CountHash<hashMD4		>,
				CountHash<hashMD5		>,
				CountHash<hashSHA1		>
			};

			return func[ha-1](data,size,hash_code);
		}

		inline bool CountAdler32(const void *data,int size,Adler32Code &hc){return CountHash<hashAdler32>(data,size,&hc);}
		inline bool CountCRC32	(const void *data,int size,CRC32Code &	hc){return CountHash<hashCRC32	>(data,size,&hc);}
		inline bool CountMD4	(const void *data,int size,MD4Code &	hc){return CountHash<hashMD4	>(data,size,&hc);}
		inline bool CountMD5	(const void *data,int size,MD5Code &	hc){return CountHash<hashMD5	>(data,size,&hc);}
		inline bool CountSHA1	(const void *data,int size,SHA1Code &	hc){return CountHash<hashSHA1	>(data,size,&hc);}

		/**
		* 计算一段数据的Hash值
		* @param data 数据指针
		* @param size 数据长度
		* @param ha hash算法
		* @param hash_str 计算后的hash值存放处
		* @param litter 小写字母
		* @return 是否计算成功
		*/
		template<HASH_ALGORITHML ha> bool CountHashStr(const void *data,int size,UTF8String &hash_str,bool litter=true)
		{
			if(!data||size<=0)return(false);

			Hash *h=CreateHash<ha>();

			if(!h)return(false);

			uint8 *hash_code=new uint8[hash_code_bytes[ha]];
			char *hash_code_str=new char[1+(hash_code_bytes[ha]<<1)];

			h->Init();
			h->Update(data,size);
			h->Final(hash_code);

			delete h;

			DataToHexStr(hash_code_str,hash_code,hash_code_bytes[ha],litter?LowerHexChar:UpperHexChar);

			hash_str.Set(hash_code_str,hash_code_bytes[ha]<<1);

			delete[] hash_code_str;
			delete[] hash_code;
			return(true);
		}

		/**
		* 计算一段数据的Hash值
		* @param data 数据指针
		* @param size 数据长度
		* @param ha hash算法
		* @param hash_str 计算后的hash值存放处
		* @param litter 小写字母
		* @return 是否计算成功
		*/
		inline bool CountHash(const void *data,int size,HASH_ALGORITHML ha,UTF8String &hash_str,bool litter=true)
		{
			if(!data||size<=0||ha<=hashNone||ha>=hashEnd)return(false);

			using CountHashFunc=bool (*)(const void *,int size,UTF8String &,bool);

			const CountHashFunc func[hashEnd-1]=
			{
				CountHashStr<hashAdler32	>,
				CountHashStr<hashCRC32		>,
				CountHashStr<hashMD4		>,
				CountHashStr<hashMD5		>,
				CountHashStr<hashSHA1		>
			};

			return func[ha-1](data,size,hash_str,litter);
		}

		inline bool CountAdler32(const void *data,int size,UTF8String &hash_str,bool litter=true){return CountHashStr<hashAdler32	>(data,size,hash_str,litter);}
		inline bool CountCRC32	(const void *data,int size,UTF8String &hash_str,bool litter=true){return CountHashStr<hashCRC32		>(data,size,hash_str,litter);}
		inline bool CountMD4	(const void *data,int size,UTF8String &hash_str,bool litter=true){return CountHashStr<hashMD4		>(data,size,hash_str,litter);}
		inline bool CountMD5	(const void *data,int size,UTF8String &hash_str,bool litter=true){return CountHashStr<hashMD5		>(data,size,hash_str,litter);}
		inline bool CountSHA1	(const void *data,int size,UTF8String &hash_str,bool litter=true){return CountHashStr<hashSHA1		>(data,size,hash_str,litter);}

		inline bool CountAdler32(const UTF8String &str,UTF8String &hash_str,bool litter=true){return CountHashStr<hashAdler32	>(str.c_str(),str.Length(),hash_str,litter);}
		inline bool CountCRC32	(const UTF8String &str,UTF8String &hash_str,bool litter=true){return CountHashStr<hashCRC32		>(str.c_str(),str.Length(),hash_str,litter);}
		inline bool CountMD4	(const UTF8String &str,UTF8String &hash_str,bool litter=true){return CountHashStr<hashMD4		>(str.c_str(),str.Length(),hash_str,litter);}
		inline bool CountMD5	(const UTF8String &str,UTF8String &hash_str,bool litter=true){return CountHashStr<hashMD5		>(str.c_str(),str.Length(),hash_str,litter);}
		inline bool CountSHA1	(const UTF8String &str,UTF8String &hash_str,bool litter=true){return CountHashStr<hashSHA1		>(str.c_str(),str.Length(),hash_str,litter);}

		/**
		* 取得一个文件的hash值
		* @param filename 文件名
		* @param ha hash算法
		* @param hash_code 计算后的hash存放处
		* @return 是否计算成功
		*/
		bool GetFileHash(const OSString &,HASH_ALGORITHML ha,void *hash_code);

		/**
		* 取得一个文件的hash值
		* @param filename 文件名
		* @param ha hash算法
		* @param hash_str 计算后的hash值存放处
		* @param litter 小写字母
		* @return 是否计算成功
		*/
		bool GetFileHash(const OSString &,HASH_ALGORITHML ha,UTF8String &hash_str,bool litter=true);
	}//namespace util
}//namespace hgl
#endif//HGL_UTIL_HASH_INCLUDE
