#include"DevilModule.h"
#include"DevilParse.h"
#include"DevilFunc.h"

namespace hgl
{
	/**
	* 映射一个属性
	* @param intro 属性在脚本语言中的描述,如"int value","string name"等
	* @param address 属性的地址
	* @return 是否创建映射成功
	*/
	bool DevilScriptModule::MapProperty(const u16char *intro,void *address)
	{
    	DevilParse parse(this,intro);
		eTokenType type;
		UTF16String name;

		type=parse.GetToken(name);

		parse.GetToken(name);

		if(prop_map.Find(name)!=-1)					//在已有记录找到同名的映射
		{
            LOG_ERROR(u"重复属性名映射: "+UTF16String(intro));
			return(false);
		}
		else
		{
            LOG_INFO(u"映射属性: "+UTF16String(intro));
		}

		{
			DevilPropertyMap *dpm=new DevilPropertyMap;

			dpm->type=type;
			dpm->address=address;

			prop_map.Add(name,dpm);

			return(true);
		}
	}

	bool DevilScriptModule::_MapFunc(const u16char *intro,void *this_pointer,void *func_pointer)
	{
    	DevilParse parse(this,intro);
		eTokenType type;
		DevilFuncMap *dfm;

		UTF16String name,func_name;

		type=parse.GetToken(name);						//取得函数返回类型

		parse.GetToken(func_name);						//取得函数名称

		if(func_map.Find(func_name)!=-1)				//在已有记录找到同名的映射
		{
			LOG_ERROR(u"repeat func name:"+UTF16String(intro));
			return(false);
		}
//		else DebugLog(u"映射函数<%s>",intro);

		#ifdef _DEBUG
			UTF16String func_intro;

			func_intro=UTF16String(GetTokenName(type))+u" "+func_name+u"(";
                //.Sprintf(u"%8s %s(",GetTokenName(type),func_name.wc_str());
		#endif//

		dfm=new DevilFuncMap;

		dfm->base	=this_pointer;
		dfm->func	=func_pointer;
		dfm->result	=type;

		parse.GetToken(ttOpenParanthesis,name);		//找到左括号为止

		while(true)
		{
			type=parse.GetToken(name);

			switch(type)
			{
				case ttCloseParanthesis:func_map.Add(func_name,dfm);		//找到右括号
										#ifdef _DEBUG
                                        	func_intro.Strcat(L')');

											LOG_INFO(u"映射函数成功，参数"+UTF16String(dfm->param.GetCount())+u"个:"+func_intro);
										#endif//_DEBUG
										return(true);
				case ttBool:

				case ttInt:
				case ttInt8:
				case ttInt16:
//				case ttInt64:
				case ttUInt:
				case ttUInt8:
				case ttUInt16:
//				case ttUInt64:

				case ttFloat:
//				case ttDouble:

				case ttString:
										#ifdef _DEBUG
											if(dfm->param.GetCount())
												func_intro.Strcat(L',');

											func_intro.Strcat(GetTokenName(type));
										#endif//_DEBUG

										dfm->param.Add(type);				//增加一个参数类型项
										break;

				case ttEnd:				delete dfm;
										return(false);
			}
		}
	}

	/**
	* 映射一个C函数
	* @param intro 函数描述，如“int getvalue(int,string)”，注意不可以写成“int getvalue(int index,string value)”
	* @param func_pointer 函数指针
	* @return 是否映射成功
	*/
	bool DevilScriptModule::MapFunc(const u16char *intro,void *func_pointer)
	{
		return _MapFunc(intro,nullptr,func_pointer);
	}

	///**
	//* 映射一个C函数，并传入一个数据
	//* @param data 传入的数据
	//* @param intro 函数描述，如“int getvalue(int,string)”，注意不可以写成“int getvalue(int index,string value)”
	//* @param func_pointer 函数指针
	//* @return 是否映射成功
	//*/
	//bool DevilScriptModule::MapFunc(void *data,const u16char *intro,void *func_pointer)
	//{
	//	return _MapFunc(DevilFuncMap::fcmFirstObject,	intro,data,func_pointer);
	//}

	/**
	* 映射一个C++成员函数
	* @param intro 函数描述，如“int getvalue(int,string)”，注意不可以写成“int getvalue(int index,string value)”
	* @param this_pointer 对象指针
	* @param func_pointer 函数指针
	* @return 是否映射成功
	*/
	bool DevilScriptModule::MapFunc(const u16char *intro,void *this_pointer,void *func_pointer)
	{
		return _MapFunc(intro,this_pointer,func_pointer);
	}

	DevilFunc *DevilScriptModule::GetScriptFunc(const UTF16String &name)
	{
		DevilFunc *func;

		if(script_func.Get(name,func))
			return func;

		LOG_ERROR(u"没有找到指定脚本函数: "+name);
        return(nullptr);
	}

	DevilFuncMap *DevilScriptModule::GetFuncMap(const UTF16String &name)
	{
		DevilFuncMap *func;

		if(func_map.Get(name,func))
			return func;
		else
        	return(nullptr);
	}

	bool DevilScriptModule::AddEnum(const u16char *enum_name,DevilEnum *script_enum)
	{
		if(enum_map.Find(enum_name)!=-1)
		{
            LOG_ERROR(u"枚举名称重复: "+UTF16String(enum_name));
			return(false);
		}

		enum_map.Add(enum_name,script_enum);
		return(true);
	}

	DevilPropertyMap *DevilScriptModule::GetPropertyMap(const UTF16String &name)
	{
		DevilPropertyMap *dpm;

		if(prop_map.Get(name,dpm))
			return dpm;
		else
        	return(nullptr);
	}

	/**
	* 添加脚本并编译
	* @param source 脚本
	* @param source_length 脚本长度，-1表示自动检测
	* @return 是否添加并编译成功
	*/
	bool DevilScriptModule::AddScript(const u16char *source,int source_length)
	{
		if(!source)return(false);

		if(source_length==-1)
			source_length=strlen(source);

		if(source_length<1)
			return(false);

		DevilParse parse(this,source,source_length);
		UTF16String name;

		while(true)
		{
			eTokenType type=parse.GetToken(name);				//不停的通过func关键字查找函数

			if(type==ttFunc)
			{
				parse.GetToken(name);							//取得函数名

				if(script_func.Find(name)==-1)					//查找是否有同样的函数名存在
				{
					DevilFunc *func=new DevilFunc(this,name);

					LOG_INFO(u"func "+name+u"()\n{");

					if(parse.ParseFunc(func))					//解析函数
					{
						script_func.Add(name,func);

						LOG_INFO(u"}\n");
					}
					else
					{
						delete func;

                        LOG_ERROR(u"解晰函数失败: "+name);
						return(false);
					}
				}
				else
				{
					LOG_ERROR(u"脚本函数名称重复: "+name);
					return(false);
				}

				continue;
			}//if type == ttFunc
			else
			if(type==ttEnum)
			{
//                parse.ParseEnum();
			}//if type == ttEnum
			else
			if(type==ttConst)
			{
			}
			else
				break;
		}//while

		return(true);
	}

	void DevilScriptModule::Clear()
	{
		script_func.Clear();
		string_list.Clear();
	}

#ifdef _DEBUG
	void DevilScriptModule::LogPropertyList()
	{
		int n=prop_map.GetCount();

		LOG_INFO(u"\n映射属性列表数量:"+UTF16String(n));

		for(int i=0;i<n;i++)
		{
			UTF16String name;
			DevilPropertyMap *dpm;

            prop_map.Get(i,name,dpm);

            LOG_INFO( u"\t"+UTF16String(i)+
                       u"\t"+UTF16String(GetTokenName(dpm->type))+
                       u"\t"+name);
		}
	}

	void DevilScriptModule::LogMapFuncList()
	{
		int n=func_map.GetCount();

		LOG_INFO(u"\n映射函数列表数量:"+UTF16String(n));

		for(int i=0;i<n;i++)
		{
			UTF16String str,name;
			DevilFuncMap *dfm;

			func_map.Get(i,name,dfm);

			//str.Sprintf(u"\t%d\t%8s %s(",i,GetTokenName(dfm->result),name.wc_str());
            str=u'\t'+UTF16String(i)+u'\t'+UTF16String(GetTokenName(dfm->result))+u' '+name+u'(';

			for(int j=0;j<dfm->param.GetCount();j++)
			{
				str.Strcat(GetTokenName(dfm->param[j]));

				if(j<dfm->param.GetCount()-1)
					str.Strcat(L',');
			}

			str.Strcat(L')');

			LOG_INFO(str);
		}
	}

	void DevilScriptModule::LogScriptFuncList()
	{
		int n=script_func.GetCount();

		LOG_INFO(u"\n脚本函数列表数量:"+UTF16String(n));

		for(int i=0;i<n;i++)
		{
			UTF16String name;
			DevilFunc *df;

			script_func.Get(i,name,df);

            LOG_INFO(u"\t"+UTF16String(i)+u"\t"+name);
		}
	}
#endif//_DEBUG
}//namespace hgl
