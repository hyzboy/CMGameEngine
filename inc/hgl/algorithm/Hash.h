#ifndef HGL_ALGORITHM_HASH_INCLUDE
#define HGL_ALGORITHM_HASH_INCLUDE

#include<hgl/type/BaseString.h>
namespace hgl
{
    namespace algorithm
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
            hashSHA1LE,                 ///<SHA1简单修改
//             hashSHA224,
//             hashSHA256,
//             hashSHA384,
//             hashSHA512,

            hashEnd
        };//enum HASH_ALGORITHML

        /**
         * Hash编码结构模板
         */
        template<int SIZE> struct HashCode
        {
            unsigned char code[SIZE];

        public:

            template<typename T>
            using CharArray=T[(SIZE<<1)+1];

        public:

            HashCode()
            {
                hgl_zero(code);
            }

            static constexpr int size(){return SIZE;}

            const unsigned char operator[](int index)const
            {
                return code[index];
            }

            void CopyFrom(const void *ptr)
            {
                memcpy(code,ptr,SIZE);
            }

            void FromString(const char *str)
            {
                ParseHexStr(code,str,SIZE);
            }

            template<typename T>
            void ToUpperString(T *str,const T gap_char=0) const
            {
                ToUpperHexStr<T>(str,code,SIZE,gap_char);
            }

            template<typename T>
            void ToLowerString(T *str,const T gap_char=0) const
            {
                ToLowerHexStr<T>(str,code,SIZE,gap_char);
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

        using HashCodeCRC32     =HashCode<4>     ;
        using HashCodeAdler32   =HashCode<4>     ;
        using HashCodeMD5       =HashCode<16>    ;
        using HashCodeMD4       =HashCode<16>    ;
        using HashCodeSHA1      =HashCode<20>    ;
        using HashCodeSHA1LE    =HashCode<20>    ;
//         using HashCodeSHA224    =HashCode<28>    ;
//         using HashCodeSHA256    =HashCode<32>    ;
//         using HashCodeSHA384    =HashCode<48>    ;
//         using HashCodeSHA512    =HashCode<64>    ;

        const int hash_code_bytes[]={0,4,4,16,16,20,20,28,32,48,64};		//hash码长度

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

        template<HASH_ALGORITHML ha> Hash *CreateHash();					///<创建一个hash值计算类实例

#define HGL_CREATE_HASH_FUNC(name)  Hash *Create##name##Hash(); \
                                    template<> inline Hash *CreateHash<hash##name>(){return Create##name##Hash();}

        HGL_CREATE_HASH_FUNC(Adler32)
        HGL_CREATE_HASH_FUNC(CRC32)
        HGL_CREATE_HASH_FUNC(MD4)
        HGL_CREATE_HASH_FUNC(MD5)
        HGL_CREATE_HASH_FUNC(SHA1)
        HGL_CREATE_HASH_FUNC(SHA1LE)
//         HGL_CREATE_HASH_FUNC(SHA224)
//         HGL_CREATE_HASH_FUNC(SHA256)
//         HGL_CREATE_HASH_FUNC(SHA384)
//         HGL_CREATE_HASH_FUNC(SHA512)

#undef HGL_CREATE_HASH_FUNC

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
                CreateSHA1Hash,
                CreateSHA1LEHash,
//                 CreateSHA224Hash,
//                 CreateSHA256Hash,
//                 CreateSHA384Hash,
//                 CreateSHA512Hash
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
                CountHash<hashSHA1		>,
                CountHash<hashSHA1LE	>,
//                 CountHash<hashSHA224	>,
//                 CountHash<hashSHA256	>,
//                 CountHash<hashSHA384	>,
//                 CountHash<hashSHA512	>
            };

            return func[ha-1](data,size,hash_code);
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
                CountHashStr<hashAdler32>,
                CountHashStr<hashCRC32	>,
                CountHashStr<hashMD4	>,
                CountHashStr<hashMD5	>,
                CountHashStr<hashSHA1	>,
                CountHashStr<hashSHA1LE	>,
//                 CountHashStr<hashSHA224	>,
//                 CountHashStr<hashSHA256	>,
//                 CountHashStr<hashSHA384	>,
//                 CountHashStr<hashSHA512	>
            };

            return func[ha-1](data,size,hash_str,litter);
        }

#define HGL_COUNT_HASH_FUNC(name)   inline bool Count##name(const void *data, int size, HashCode##name &hc) { return CountHash<hash##name>(data, size, &hc); }    \
                                    inline bool Count##name(const void *data, int size, UTF8String &hash_str, bool litter = true) { return CountHashStr<hash##name>(data, size, hash_str, litter); }  \
                                    inline bool Count##name(const UTF8String &str, UTF8String &hash_str, bool litter = true) { return CountHashStr<hash##name>(str.c_str(), str.Length(), hash_str, litter); }

        HGL_COUNT_HASH_FUNC(Adler32)
        HGL_COUNT_HASH_FUNC(CRC32)
        HGL_COUNT_HASH_FUNC(MD4)
        HGL_COUNT_HASH_FUNC(MD5)
        HGL_COUNT_HASH_FUNC(SHA1)
        HGL_COUNT_HASH_FUNC(SHA1LE)
//         HGL_COUNT_HASH_FUNC(SHA224)
//         HGL_COUNT_HASH_FUNC(SHA256)
//         HGL_COUNT_HASH_FUNC(SHA384)
//         HGL_COUNT_HASH_FUNC(SHA512)

#undef HGL_COUNT_HASH_FUNC

        /**
        * 取得一个文件的hash值
        * @param filename 文件名
        * @param ha hash算法
        * @param hash_code 计算后的hash存放处
        * @return 是否计算成功
        */
        bool GetFileHash(const OSString &filename,HASH_ALGORITHML ha,void *hash_code);

        /**
        * 取得一个文件的hash值
        * @param filename 文件名
        * @param ha hash算法
        * @param hash_str 计算后的hash值存放处
        * @param litter 小写字母
        * @return 是否计算成功
        */
        bool GetFileHash(const OSString &filename,HASH_ALGORITHML ha,UTF8String &hash_str,bool litter=true);
    }//namespace algorithm
}//namespace hgl
#endif//HGL_ALGORITHM_HASH_INCLUDE
