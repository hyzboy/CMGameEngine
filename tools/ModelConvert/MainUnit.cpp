#include<hgl/Console.h>				//GraphicsApplication,SystemInitInfo
#include<hgl/object/FlowObject.h>	//FlowObject
#include<hgl/ut/CmdParse.h>			//CmadParse

#include"AssimpLoader.h"

using namespace hgl;
using namespace hgl::util;
using namespace hgl::graph;

HGL_CONSOLE_MAIN(sii,app,args)
{	
	CmdParse cmd(args);
	
	sii.info.ProjectName=U8_TEXT("模型转换");
	sii.info.ProjectCode=OS_TEXT("Model Converter");
	sii.info.ProjectVersion=U8_TEXT("1.00");
    
	if(!app.Init(&sii))return(-1);

    LOG_INFO("argc="+UTF8String(args.GetCount()));

    for(int i=0;i<args.GetCount();i++)
        LOG_INFO(OS_TEXT("argv[")+OSString(i)+OS_TEXT("] ")+args[i]);

	if(args.GetCount()<=1)
	{
		LOG_INFO("Model Converter\n"
				"\n"
				"Example: ModelConvert <model filename>");

		return 0;
	}

	AssimpLoader al;

	al.LoadFile(args[1]);

	return(0);
}
