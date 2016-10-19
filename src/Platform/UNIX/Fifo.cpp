#include<hgl/proc/Fifo.h>
#include<hgl/Str.h>
#include<sys/stat.h>

namespace hgl
{
	/**
	 * 创建一个管道通信文件
	 * @param fn 简略文件名(请不要带路径)
	 * @return 是否创建成功
	 */
	bool Fifo::Create(const char *fn)
	{
		if(!fn||!(*fn))
			return(false);

		hgl::strcpy(filename,HGL_MAX_PATH-5,"/tmp/");
		hgl::strcpy(filename+5,HGL_MAX_PATH-5,fn);

		fd=mkfifo(filename,0777);

		return(!fd);
	}
}//namespace hgl
