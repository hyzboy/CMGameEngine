#include<hgl/Graphics.h>
#include<hgl/platform/GraphicsApplication.h>
#include<hgl/type/StringList.h>

using namespace hgl;
using namespace hgl::graph;

int main(int argc,char **argv)
{
#ifdef _DEBUG
	try
	{
#endif//_DEBUG
		OSStringList sl;

		for(int i=0;i<argc;i++)
			sl.Add(argv[i]);

		GraphicsSystemInitInfo sii;
		GraphicsApplication app;

		return GraphicsAppMain(sii,app,sl);
#ifdef _DEBUG
	}
	catch(...)
	{
		return(-1);
	}
#endif//_DEBUG
}
