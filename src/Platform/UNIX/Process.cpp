#include<hgl/proc/Proc.h>
#include<hgl/File.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>

namespace hgl
{
	bool Process::SetWorkPath(const OSString &wp)
	{
		if(!IsDirectory(wp))
			return(false);

		work_path=wp;
		return(true);
	}

	bool Process::SetExecFile(const OSString &ef)
	{
		if(!FileCanExec(ef))
			return(false);

		filename=ef;
		return(true);
	}

	bool Process::Execute()
	{
		pid=fork();

		if(pid<0)
			_exit(72);		//为啥是72，我也不清楚，反正是要退出，多少无所谓。抄的poco

		if(pid==0)
		{	//fork会复制出一个子进程并从这里开始执行
			if(chdir(work_path.c_str())!=0)
				_exit(72);

			char **argv=new char *[args.GetCount()+2];

			argv[0]=filename;
			for(int i=0;i<args.GetCount();i++)
				argv[i+1]=args[i].c_str();
			argv[args.GetCount()+1]=0;

			execvp(filename.c_str(),argv);
			_exit(72);
		}
		else	//原本的父进程
		{
		}

		return(true);
	}

	bool Process::Wait()
	{
		if(pid<0)return(false);
		int status;
		int rc;

		do
		{
			rc=waitpid(pid,&status,0);
		}while(rc<0&&errno==EINTR);

		return(true);
	}

	bool Process::Kill()
	{
		if(pid<0)return(false);

		return !kill(pid,SIGKILL);
	}

	bool Process::RequestTerminate()
	{
		if(pid<0)return(false);

		return !kill(pid,SIGINT);
	}
}//namespace hgl
