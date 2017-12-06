#ifndef HGL_ALGORITHM_CRYPT_INCLUDE
#define HGL_ALGORITHM_CRYPT_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl   //校验/加密算法
{
    namespace io
    {
        class OutputStream;
    }//namespace io

    namespace algorithm
    {
        void OverflowEncrypt(void *, void *, int, void *, int);			        ///<溢出加密(轻度加密)
        void OverflowDecrypt(void *, void *, int, void *, int);			        ///<溢出解密

        void AesEncrypt(uint8 data[16], uint8 *key, int keysize);               ///<AES加密
        void AesDecrypt(uint8 data[16], uint8 *key, int keysize);               ///<AES解密

        void RC4Encrypt(uint8 *data, int datasize, uint8 *key, int keysize);    ///<RC4加密

        bool base64_encode(io::OutputStream *os,const uchar *input, size_t len);
        bool base64_decode(io::OutputStream *os,const uchar *input,size_t len);
    }//namespace algorithm
}//namespace hgl
#endif//HGL_ALGORITHM_CRYPT_INCLUDE
