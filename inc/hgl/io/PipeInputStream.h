#ifndef HGL_IO_PIPE_INPUT_STREAM_INCLUDE
#define HGL_IO_PIPE_INPUT_STREAM_INCLUDE

namespace hgl
{
	namespace io
	{
		/**
		 * 管道输入流
		 */
		class PipeInputStream:public InputStream
		{
			int pipe_fd;

		public:

			PipeInputStream(){pipe_fd=-1;}
			PipeInputStream(int fd){pipe_fd=fd;}

			void Init(int fd){pipe_fd=fd;}
			void Close(){pipe_fd=-1;}

		};//class PipeInputStream
	}//namespace io
}//namespace hgl
#endif//HGL_IO_PIPE_INPUT_STREAM_INCLUDE
