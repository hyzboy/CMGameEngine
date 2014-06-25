#ifndef HGL_IO_COMPRESS_OUTPUT_STREAM_INCLUDE
#define HGL_IO_COMPRESS_OUTPUT_STREAM_INCLUDE

#include<hgl/io/OutputStream.h>
namespace hgl
{
	namespace io
	{
		/**
		* 压缩输出流</p>
		* 写入原始数据，将自动得到压缩后的数据。
		*/
		class CompressOutputStream:public OutputStream
		{
		public:
		};//class CompressOutputStream
	}//namespace io
}//namespace hgl
#endif//HGL_IO_COMPRESS_OUTPUT_STREAM_INCLUDE
