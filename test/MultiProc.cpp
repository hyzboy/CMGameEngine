#include<hgl/proc/Proc.h>

using namespace hgl;

int main(int,char **)
{
	const char *wp[32]=
	{
		"/",
		"/home",
		"/usr"
	};

	const char *argv[32]=
	{
		"root",
		"home",
		"user path"
	};

	Process p[3];

	for(int i=0;i<3;i++)
	{
		p[i].SetWorkPath(wp[i]);
		p[i].SetExecFile("/home/hyzboy/workspace/cm/test/a.out");
		p[i].AddArgv(argv[i]);

		p[i].Execute();
	}

	return(3);
}
