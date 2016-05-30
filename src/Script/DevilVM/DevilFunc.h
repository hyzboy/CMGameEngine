#ifndef DevilFuncH
#define DevilFuncH

#include"DevilCommand.h"
#include<hgl/type/BaseString.h>
#include<hgl/type/Map.h>
namespace hgl
{
	class DevilScriptModule;

	/**
	* 虚拟机内脚本函数定义
	*/
	class DevilFunc
	{
		DevilScriptModule *module;

	public:

    	UTF16String func_name;

		ObjectList<DevilCommand> command;

		Map<UTF16String,int> goto_flag;

		MapObject<UTF16String,DevilValueInterface> script_value_list;

	public:

		DevilFunc(DevilScriptModule *dvm,const UTF16String &name){module=dvm;func_name=name;}

		bool AddGotoFlag(const UTF16String &);		//增加跳转旗标
		int FindGotoFlag(const UTF16String &);		//查找跳转旗标

		void AddGotoCommand(const UTF16String &);	//增加跳转指令
		void AddReturn();							//增加返回指令

		int AddCommand(DevilCommand *cmd)			//直接增加指令
		{
			return command.Add(cmd);
		}

		void AddScriptFuncCall(DevilFunc *);		//增加脚本函数呼叫

		DevilValueInterface *AddValue(eTokenType,const UTF16String &);			//增加一个变量
	};//class DevilFunc
}//namespace hgl
#endif
