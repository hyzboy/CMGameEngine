#ifndef HGL_MULTI_PROC_PIPE_INCLUDE
#define HGL_MULTI_PROC_PIPE_INCLUDE

namespace hgl
{
	/**
	 * 进程管道通信
	 */
	class Pipe																						///进程Pipe管道通信
	{
	public:

		union pipe_data
		{
			int file_pipes[2];

			struct
			{
				int read_pipe;
				int write_pipe;
			};
		}pd;

	public:

		Pipe()
		{
			pd.read_pipe=-1;
			pd.write_pipe=-1;
		}

		virtual ~Pipe();

		bool Create();																				///<创建一个pipe通信管道
	};//class Pipe
}//namespace hgl
#endif//HGL_MULTI_PROC_PIPE_INCLUDE
