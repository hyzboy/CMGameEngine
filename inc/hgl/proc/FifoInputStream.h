#ifndef HGL_MULTI_PROC_FIFO_INPUT_STREAM_INCLUDE
#define HGL_MULTI_PROC_FIFO_INPUT_STREAM_INCLUDE

#include<hgl/Fifo.h>
#include<hgl/io/InputStream.h>

namespace hgl
{
	namespace io
	{
		/**
		 * 命名管道输入流
		 */
		class FifoInputStream:public InputStream
		{
			Fifo *f;

		public:

			FifoInputStream(Fifo *_f)
			{
				f=_f;
			}

			virtual ~FifoInputStream()
			{
				if(f)
					delete f;
			}

			
		};//class FifoInputStream
	}//namespace io
}//namespace hgl
#endif//HGL_MULTI_PROC_FIFO_INPUT_STREAM_INCLUDE
