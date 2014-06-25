#include<stdio.h>
#include<embryo-1/Embryo.h>
#include<hgl/Console.h>
#include<hgl/File.h>

using namespace hgl;

typedef Embryo_Cell (*EmbryoNativeCallFunc) (Embryo_Program *ep, Embryo_Cell *params);

class EmbryoFuncMap
{
};

/**
 * Embryo 抽象机封装基类
 */
class EmbryoAM
{
	Embryo_Program *ep;

	char *bytecode;
	int bytecode_size;

public:

	EmbryoAM()
	{
		ep=nullptr;
	}

	~EmbryoAM()
	{
		if(ep)
			embryo_program_free(ep);

		SAFE_CLEAR_ARRAY(bytecode);
	}

	bool Load(const char *filename)
	{
		if(ep)
			return(false);

		bytecode_size=hgl::LoadFileToMemory(filename,(void **)&bytecode);

		if(bytecode_size<=0)
			return(false);

		ep=embryo_program_const_new(bytecode,bytecode_size);		//const版函数的意思是我方不会释放它

		if(!ep)
		{
			delete[] bytecode;
			return(false);
		}

		return(true);
	}

	bool MapFunc(const char *funcname,void *funcaddress)
	{
		embryo_program_native_call_add(ep,funcname,(EmbryoNativeCallFunc)funcaddress);
		return(true);
	}
};//class EmbryoAM

HGL_CONSOLE_MAIN_FUNC()
{
	printf("Embryo Script test\n\n");

	if(args.GetCount()<2)
	{
		printf("usage: es_test 1.ebc\n\n");
		return(-1);
	}

	printf("script bytecode file: %s\n",args[1].c_str());

	embryo_init();

	EmbryoAM eam;

	if(!eam.Load(args[1].c_str()))
	{
		printf("embryo_program_load failed.\n");
		return(0);
	}

	printf("embryo_program_free\n");
	embryo_shutdown();
	return(0);
}
