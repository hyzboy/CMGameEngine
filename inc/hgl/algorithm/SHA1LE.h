#ifndef HGL_ALGORITHM_HASH_SHA1LE_INCLUDE
#define HGL_ALGORITHM_HASH_SHA1LE_INCLUDE

#include<hgl/algorithm/Hash.h>
namespace hgl
{
    namespace algorithm
    {
        /**
         * SHA1-LE Hash算法<br>
         * 算法保持SHA1不变，改动如下:<br>
         *  1.去掉了因BigEndian设计造成的LittleEndian处理器需要做大小头转换的部分
         *  2.可自定义起始因子
         *  3.可动态修改扰乱因子
         */
        class SHA1LE:public Hash
        {
            enum
            {
                BLOCK_SIZE  = 64,
                DIGEST_SIZE = 20
            };

            uint32 digest[5]; // Message digest
            uint32 countLo;   // 64-bit bit count
            uint32 countHi;
            uint32 data[16];  // SHA data buffer
            uint32 slop;      // # of bytes saved in data[]

            uint32 K[4];

            uint32 W[80];

        private:

            void sha1_transform();

        public:

            void GetName(UTF8String &str)const override{str="SHA1LE";}
            void GetName(UTF16String &str)const override{str=U16_TEXT("SHA1LE");}

            const int GetHashBytes()const override {return DIGEST_SIZE;}

            void Init(const uint32 *start_digest,const uint32 *mysterious_constants);   ///<开始一次新的HASH计算，并指定初始因子和扰乱因子
            void Init()override;                                                        ///<开始一次新的HASH计算，并使用缺省初始因子和扰乱因子

            void SetMark(const uint32 *mysterious_constants);                           ///<更新扰乱因子

            void Update(const void *input,uint count)override;                          ///<添加新的HASH数据
            void Final(void *result)override;                                           ///<结束本次HASH计算
        };//class SHA1LE
    }//namespace algorithm
}//namespace hgl
#endif//HGL_ALGORITHM_HASH_SHA1LE_INCLUDE
