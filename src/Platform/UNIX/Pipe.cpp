#include<hgl/proc/Pipe.h>
#include<unistd.h>

namespace hgl
{
	Pipe::~Pipe()
	{
		if(pd.read_pipe!=-1)close(pd.read_pipe);
		if(pd.write_pipe!=-1)close(pd.write_pipe);
	}

	bool Pipe::Create()
	{
		return(pipe(pd.file_pipes)==0);
	}
}//namespace hgl
