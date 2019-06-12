#ifndef HGL_IO_DECOMPRESS_INPUT_STREAM_INCLUDE
#define HGL_IO_DECOMPRESS_INPUT_STREAM_INCLUDE

#include<hgl/io/InputStream.h>
namespace hgl
{
    namespace io
    {
        /**
        * 压缩数据输入流</p>
        * 读取压缩的数据，将直接得到解压后的数据。
        */
        class DecompressInputStream:public InputStream
        {
        public:
        };//class class DecompressInputStream
    }//namespace io
}//namespace hgl
#endif//HGL_IO_DECOMPRESS_INPUT_STREAM_INCLUDE
