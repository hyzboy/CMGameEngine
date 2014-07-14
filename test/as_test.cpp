#include<hgl/Console.h>
#include<hgl/script/AngelVM.h>
#include<unistd.h>
#include<string>

using namespace hgl;

void out_str(const UTF8String &str)
{
	write(STDOUT_FILENO,str.c_str(),str.Length());
	write(STDOUT_FILENO,"\n",1);
}

class TestObject
{
public:

	int c;

public:

	TestObject()
	{
		c=0;
	}

	void put_str(int a,int b)
	{
		out_str(UTF8String("a+b+c=")+UTF8String(a+b+c));
	}
};//class TestObject

DEF_ANGEL_OBJECT(TestOut,TestObject)

const char script[]=u8"void main()				\n"
					u8"{						\n"
					u8" TestOut to;				\n"
					u8"\n"
					u8"	print(\"hello,world!\");\n"
					u8"	print(\"你好，中国!\");	\n"
					u8"\n"
					u8" to.c=3;					\n"
					u8" to.put_str(1,2);		\n"
					u8"}";

using namespace hgl;
HGL_CONSOLE_MAIN_FUNC()
{
	sii.info.ProjectName=OS_TEXT("AngelScript Test");	//中文名称,用于各处对人的名称
	sii.info.ProjectCode=OS_TEXT("AngelScriptTest");	//工程代码,用于文件名，目录，注册表，卷标等对程序处

//	sii.log.file=false;									//关闭文件日志

	if(!app.Init(&sii))
		return(-1);

	AngelVM vm;

	vm.MapFunc("void print(const string &in)",(void *)&out_str);

	AngelObject *ao=CreateAngelObject((&vm),TestOut,TestObject);

	ao->BindObjectProperty("int c",TestObject,c);
	ao->BindObjectFunc("void put_str(int,int)",TestObject,put_str);

	AngelModule *module=vm.GetModule();

	module->AddScript(__FILE__,script,strlen(script));

	module->Build();

	AngelContext *context=module->CreateContext("main");

	context->Start();

	return 0;
}
