#include"DevilFunc.h"
#include"DevilModule.h"

namespace hgl
{
	bool DevilFunc::AddGotoFlag(const UTF16String &name)
	{
		int count=command.GetCount();
		int index=goto_flag.Find(name);

		if(index==-1)
		{
			goto_flag.Add(name,count);

            LOG_INFO(u":"+name);

			return(true);
		}
		else
		{
			LOG_INFO(u"添加跳转标识符失败，这个标识符重复了:"+name);

			return(false);
		}
	}

	int DevilFunc::FindGotoFlag(const UTF16String &name)
	{
		int index;

		if(goto_flag.Get(name,index))
			return(index);

		return -1;
	}

	void DevilFunc::AddGotoCommand(const UTF16String &name)
	{
		#ifdef _DEBUG
		const int index=command.Add(new DevilGoto(module,this,name));

		LOG_INFO(UTF16String(index)+u"\tgoto "+name+u";");
		#else
		command.Add(new DevilGoto(module,this,name));
		#endif//_DEBUG
	}

	void DevilFunc::AddReturn()
	{
		#ifdef _DEBUG
		const int index=command.Add(new DevilReturn(module));

        LOG_INFO(UTF16String(index)+u"\treturn;");
		#else
		command.Add(new DevilReturn(module));
		#endif//_DEBUG
	}

	void DevilFunc::AddScriptFuncCall(DevilFunc *script_func)
	{
		#ifdef _DEBUG
		const int index=command.Add(new DevilScriptFuncCall(module,script_func));

        LOG_INFO(UTF16String(index)+u"\t call "+script_func->func_name);
		#else
		command.Add(new DevilScriptFuncCall(module,script_func));
		#endif//
	}

	DevilValueInterface *DevilFunc::AddValue(TokenType type,const UTF16String &name)
	{
		if(script_value_list.Find(name)!=-1)
		{
			LOG_ERROR(u"添加变量失败，变量名称重复:"+name);

			return(nullptr);
		}

		if(type==ttBool		)return(new DevilScriptValue<bool		>(module,func_name,name,type));else
		if(type==ttString	)return(new DevilScriptValue<UTF16String>(module,func_name,name,type));else
		if(type==ttInt		)return(new DevilScriptValue<int		>(module,func_name,name,type));else
		if(type==ttUInt		)return(new DevilScriptValue<uint		>(module,func_name,name,type));else
		if(type==ttInt8		)return(new DevilScriptValue<int8		>(module,func_name,name,type));else
		if(type==ttUInt8	)return(new DevilScriptValue<uint8		>(module,func_name,name,type));else
		if(type==ttInt16	)return(new DevilScriptValue<int16		>(module,func_name,name,type));else
		if(type==ttUInt16	)return(new DevilScriptValue<uint16		>(module,func_name,name,type));else
		if(type==ttInt64	)return(new DevilScriptValue<int64		>(module,func_name,name,type));else
		if(type==ttUInt64	)return(new DevilScriptValue<uint64		>(module,func_name,name,type));else
		if(type==ttFloat	)return(new DevilScriptValue<float		>(module,func_name,name,type));else
		if(type==ttDouble	)return(new DevilScriptValue<double		>(module,func_name,name,type));else
		{
			LOG_ERROR(u"变量类型无法识别,name="+name+u",id="+UTF16String(type));
			return(nullptr);
		}
	}
}//namespace hgl

