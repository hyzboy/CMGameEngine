#include"DevilParse.h"
#include"DevilModule.h"
#include<hgl/LogInfo.h>

namespace hgl
{
	void ConvertString(UTF16String &targe,u16char *source,int length)
	{
		u16char *temp;
		u16char *sp,*tp;
		const u16char conv[][2]=
		{
			{'t',	'\t'},
			{'n',	'\n'},
			{'r',	'\r'},
			{'"',	'"'},
			{'\\',	'\\'},
			{'\'',	'\''},
			{0,0}
		};

		temp=new u16char[length+1];
		sp=source;
		tp=temp;

		while(length)
		{
			if(*sp=='\\')
			{
				u16char sc=*(sp+1);
				int select;

				for(select=0;;select++)
					if(conv[select][0]==0)break;
					else
					if(conv[select][0]==sc)
					{
						*tp++=conv[select][1];

						break;
					}

				if(!conv[select][0])		//没选中什么
				{
					*tp++='\\';
					*tp++=sc;
				}

				length-=2;
				sp+=2;
			}
			else
			{
				*tp++=*sp++;

				length--;
			}
		}

		*tp=0;
		targe=temp;

		delete[] temp;
	}
}

namespace hgl
{
	DevilParse::DevilParse(DevilScriptModule *dm,const u16char *str,int len)
	{
    	module=dm;

		source_start=str;
		source_cur=str;

		if(len==-1)
			source_length=strlen(str);
		else
			source_length=len;
	}

	eTokenType DevilParse::GetToken(UTF16String &intro)
	{
		while(true)
		{
			uint len;
			eTokenType type;
			const u16char *source;

			if(source_length<=0)return(ttEnd);

			type=parse.GetToken(source_cur,source_length,&len);

			source			=	source_cur;

			source_cur		+=	len;
			source_length	-=	len;

			if(type<=ttEnd)
				return ttEnd;

			if(type<=ttMultilineComment)		//跳过注释，空格，换行
				continue;

			intro.Strcpy(source,len);
			return type;
		}
	}

	eTokenType DevilParse::CheckToken(UTF16String &intro)
	{
		const u16char *cur=source_cur;
		uint len=source_length;
		eTokenType type;

		type=GetToken(intro);

		source_cur		=cur;
		source_length	=len;

		return(type);
	}

	bool DevilParse::GetToken(eTokenType tt,UTF16String &name)
	{
		eTokenType type;

		while(true)
		{
			type=GetToken(name);

			if(type==tt)
            	return(true);

			if(type<=ttEnd)
            	return(false);
		}
	}

	void DevilParse::ParseValue(DevilFunc *func,eTokenType value_type,UTF16String &/*type_name*/)
	{
		eTokenType type;

		UTF16String value_name;

		type=GetToken(value_name);

		if(type!=ttIdentifier)	//变量名称
			return;

		DevilValueInterface *dvi=func->AddValue(value_type,value_name);

		if(!dvi)
		{
            LOG_ERROR(u"函数<"+func->func_name+u">的变量<"+value_name+u">定义无法解析");
			return;
		}

		type=GetToken(value_name);

		if(type==ttEndStatement)return;	//分号
		if(type!=ttAssignment)return;	//等号

		DevilValueInterface *dvi_value=ParseValue();	//后面的值或表达式

		if(!dvi_value)
		{
			LOG_ERROR(u"函数<"+func->func_name+u">的变量<"+value_name+u">定义时的赋值式无法解析");
			return;
		}

//		dvi->SetValue(dvi_value);		//赋值

		return;
	}

/*	void DevilParse::ParseEnum()
	{
		eTokenType type;
		UTF16String name;

		type=GetToken(name);


	}*/

	bool DevilParse::ParseFunc(DevilFunc *func)
	{
		UTF16String name;

//		GetToken(ttOpenParanthesis,name);		// (
												// 脚本函数暂时不支持参数
		GetToken(ttCloseParanthesis,name);		// )

		return ParseCode(func);
	}

	bool DevilParse::ParseCode(DevilFunc *func)
	{
		UTF16String name;
		eTokenType type;
		bool ca;
		int StatmentCount;					//花括号数量

		type=CheckToken(name);

		if(type==ttStartStatementBlock)		//如果是左花括号
		{
			ca=false;						//结束时不用检测分号

			StatmentCount=1;

			GetToken(name);					//取走左花括号
		}
		else
		{
			ca=true;

			StatmentCount=0;
		}

		do
		{
			type=GetToken(name);

			if(type==ttStartStatementBlock)		// {
			{
				StatmentCount++;

				continue;
			}

			if(type==ttEndStatementBlock)		// }
			{
				StatmentCount--;
				continue;
			}

			if(type<=ttEnd)
				return(false);

			/*if(type==ttEnum)					//解析枚举
			{
				ParseEnum();

				continue;
			}*/

			if(type==ttGoto)					//goto
			{
				GetToken(name);					//取得跳转标识符

				func->AddGotoCommand(name);

				continue;
			}

			if(type==ttIf)
			{
				if(ParseIf(func))
					continue;
				else
				{
                	LOG_ERROR(u"if解析错误");
					return(false);
				}
			}

			if(type==ttReturn)
			{
				func->AddReturn();			//return

				continue;
			}

			if(type==ttBool			||type==ttString
			 ||type==ttInt			||type==ttUInt
			 ||type==ttInt8			||type==ttUInt8
			 ||type==ttInt16		||type==ttUInt16
			 ||type==ttInt64		||type==ttUInt64
			 ||type==ttFloat		||type==ttDouble)
			{
				ParseValue(func,type,name);			//解释变量定义

				continue;
			}

			if(type==ttIdentifier)				//未知标识
			{
				UTF16String temp;

				type=GetToken(temp);

				if(type==ttColon)				//冒号,Goto用标识
				{
					if(func->AddGotoFlag(name))
						continue;
					else
					{
						LOG_ERROR(u"冒号后面的字段无法解析!");
						return(false);
					}
				}
				else
				if(type==ttOpenParanthesis)		//函数
				{
					//真实函数调用验证
					{
						DevilFuncMap *map_func=module->GetFuncMap(name);

						if(map_func)
						{
							#ifdef _DEBUG
							UTF16String intro;

							DevilCommand *cmd=ParseFuncCall(name,map_func,intro);
							#else
							DevilCommand *cmd=ParseFuncCall(map_func);
							#endif//

							if(cmd)
							{
								#ifdef _DEBUG
								int index=
								#endif//_DEBUG

								func->AddCommand(cmd);

								if(module->OnTrueFuncCall!=nullptr)
								{
									bool call=CallEvent(module->OnTrueFuncCall,(name.c_str()));

									if(call)
										cmd->Run();
								}

								#ifdef _DEBUG
								LOG_INFO(UTF16String(index)+u"\t"+intro);
								#endif//_DEBUG

								continue;
							}//if cmd
						}// if map_func
					}

					//脚本函数调用验证
					{
						DevilFunc *script_func=module->GetScriptFunc(name);

						if(script_func)
						{
							func->AddScriptFuncCall(script_func);

							continue;
						}
					}

					LOG_PROBLEM(u"脚本调用函数没有找到相应的真实函数映射与脚本函数: "+name);
//					return(false);		//错误也不退出，是为了把所有不支持的函数全列出来
				}
			}

//			ErrorHint(u"无法理解的标识符:%s",name.c_str());
//			return(false);
		}
		while(StatmentCount);				//如果匹配的花括号用完了，即代表函数结束了

		if(ca)								//没有用花括号匹配，所以要取走分号
        	GetToken(ttEndStatement,name);

		return(true);
	}

	template<typename T> bool ParseToNumber(T &result,const UTF16String &str);

    template<> bool ParseToNumber<int>  (int &  result,const UTF16String &str){return str.ToInt  (result);}
    template<> bool ParseToNumber<uint> (uint & result,const UTF16String &str){return str.ToUint (result);}
    template<> bool ParseToNumber<float>(float &result,const UTF16String &str){return str.ToFloat(result);}

    template<typename T>
    bool DevilParse::ParseNumber(T &result,const UTF16String &str)           //由于从程式理论上讲，调用这个函数时，都是因为测出str是数值的时候，所以不可能产生解析错误的情况。
    {
        if(ParseToNumber<T>(result,str))
            return(true);

        LOG_ERROR(u"解析数值\""+str+u"\"失败!");
        return(false);
    }

	#ifdef _DEBUG
	DevilCommand *DevilParse::ParseFuncCall(UTF16String &func_name,DevilFuncMap *map,UTF16String &intro)
	#else
	DevilCommand *DevilParse::ParseFuncCall(DevilFuncMap *map)
	#endif//
	{
		UTF16String name;
		eTokenType type;

		//按个数解晰参数
		int i=0,param_count=map->param.GetCount();
		DevilSystemFuncParam *param,*p;

		#if HGL_CPU == HGL_CPU_X86_64
		if(map->base)
		{
			param=new DevilSystemFuncParam[param_count+1];

			param[0].void_pointer=map->base;			//x64下第一个参数放置this

			p=param+1;
		}
		#else
			param=new DevilSystemFuncParam[param_count];

			p=param;
		#endif//HGL_CPU == HGL_CPU_X86_64

		#ifdef _DEBUG
		//intro.Sprintf(u"%s(",func_name.c_str());
        intro=func_name+u'(';
		#endif//


		while(true)
		{
			if(i>=param_count)break;

			type=GetToken(name);

			if(type==ttListSeparator)continue;

			switch(map->param[i++])
			{
				case ttBool:	if(type==ttTrue)	   		*(bool *)p=true;					else
								if(type==ttFalse)	   		*(bool *)p=false;					else
								if(type==ttIntConstant)
                                {
                                    if(!ParseNumber(*(int *)p,name))
                                        break;
                                }
                                else
                                    break;

								#ifdef _DEBUG
								intro.Strcat(*(bool *)(p)?L"true":u"false");
								if(i<param_count)intro.Strcat(L',');
								#endif//

								p++;
								continue;
				case ttInt:
				case ttInt8:
				case ttInt16:
//				case ttInt64:
                            	if(type==ttIntConstant
                                 ||type==ttFloatConstant
                                 ||type==ttDoubleConstant)
                                {
                                    if(!ParseNumber(*(int *)p,name))
                                        break;
                                }
                                else
                                    break;

                                #ifdef _DEBUG
								//intro.CatSprintf(u"%d",*(int *)(p));
                                intro+=UTF16String(*(int *)p);
								if(i<param_count)intro.Strcat(L',');
								#endif//

								p++;
								continue;
				case ttUInt:
				case ttUInt8:
				case ttUInt16:
//				case ttUInt64:
								if(type==ttIntConstant
                                 ||type==ttFloatConstant
                                 ||type==ttDoubleConstant)
                                {
                                    if(!ParseNumber(*(uint *)p,name))
                                        break;
                                }
                                else
                                    break;

                                #ifdef _DEBUG
								//intro.CatSprintf(u"%u",*(uint *)(p));
                                intro+=UTF16String(*(uint *)p);
								if(i<param_count)intro.Strcat(L',');
								#endif//

								p++;
								continue;
				case ttFloat:
//				case ttDouble:
								if(type==ttIntConstant
								 ||type==ttFloatConstant
								 ||type==ttDoubleConstant)
								{
                                    if(!ParseNumber(*(float *)p,name))
                                        break;
                                }
                                else
                                    break;

                                #ifdef _DEBUG
								//intro.CatSprintf(u"%f",*(float *)(p));
                                intro+=UTF16String(*(float *)p);
								if(i<param_count)intro.Strcat(L',');
								#endif//

								p++;
								continue;

				case ttString:  if(type==ttStringConstant)
								{
									UTF16String str;
									int index;

									ConvertString(str,name.c_str()+1,name.Length()-2);		//去掉两边的引号，并转换\t\n之类的数据

									index=module->string_list.Add(str.c_str());

									*(u16char **)(p)=(u16char *)(module->string_list[index].c_str());

									#ifdef _DEBUG
									intro.Strcat(name);
									if(i<param_count)intro.Strcat(L',');
									#endif//

									p++;
									continue;
								}
								break;
			}

			delete[] param;

			#ifdef _DEBUG
			LOG_HINT(u"脚本中的参数和映射函数的格式要求不兼容！\n\t"+intro);
			#endif//

			GetToken(ttCloseParanthesis,name);		//右括号
			return(nullptr);
		}

		GetToken(ttCloseParanthesis,name);		//右括号

//		if(CheckToken(name)==ttEndStatement)	//测试下一个是否分号
//			GetToken(name);					   	//取走分号

		#if HGL_CPU == HGL_CPU_X86_64
		if(map->base)param_count++;					//如果是x64位下的C++函数，第一个参数放this指针
		#endif//HGL_CPU == HGL_CPU_X86_64

		if(map->result==ttVoid	)return(new DevilSystemFuncCallFixed<void *		>(map,param,param_count));else

		if(map->result==ttBool	)return(new DevilSystemFuncCallFixed<bool		>(map,param,param_count));else
		if(map->result==ttInt8	)return(new DevilSystemFuncCallFixed<int8		>(map,param,param_count));else
		if(map->result==ttInt16	)return(new DevilSystemFuncCallFixed<int16		>(map,param,param_count));else
		if(map->result==ttInt	)return(new DevilSystemFuncCallFixed<int32		>(map,param,param_count));else
		if(map->result==ttUInt8	)return(new DevilSystemFuncCallFixed<uint8		>(map,param,param_count));else
		if(map->result==ttUInt16)return(new DevilSystemFuncCallFixed<uint16		>(map,param,param_count));else
		if(map->result==ttUInt	)return(new DevilSystemFuncCallFixed<uint32		>(map,param,param_count));else
		if(map->result==ttFloat	)return(new DevilSystemFuncCallFixed<float		>(map,param,param_count));else
		if(map->result==ttString)return(new DevilSystemFuncCallFixed<u16char *	>(map,param,param_count));else
		{
			delete[] param;
			return(nullptr);
		}
	}

	bool DevilParse::ParseIf(DevilFunc *func)
	{
		UTF16String name;
		UTF16String flag;
		DevilCompInterface *dci;
		DevilCompGoto *dcg;

        flag=func->func_name+u'_'+UTF16String(func->command.GetCount());

		dci=ParseComp();																			//解析比较表达式

		if(!dci)
			return(false);

        func->command.Add(dcg=new DevilCompGoto(module,dci,func));									//增加比较跳转控制

		LOG_INFO(u"if "+flag);

		ParseCode(func);																			//解析 then 段

		if(CheckToken(name)==ttElse)
		{
			func->AddGotoCommand(flag+u"_end");														//在else前增加goto到if/else结束处

			GetToken(ttElse,name);

			dcg->else_flag=flag+u"_else";															//设置比较else的话跳到else段

			func->AddGotoFlag(flag+u"_else");														//增加else段跳转旗标

			ParseCode(func);																		//解析 else 段
		}
		else
			dcg->else_flag=flag+u"_end";															//设置比较else的话直接跳到最后

		func->AddGotoFlag(flag+u"_end");															//增加结束跳转用旗标

    	return(true);
	}

	DevilCompInterface *DevilParse::ParseComp()
	{
		UTF16String name;
		int comp;
		DevilValueInterface *left,*right;

		GetToken(ttOpenParanthesis,name);		// (

		//解析比较式左边
		left=ParseValue();

		if(!left)return(nullptr);

		//解析比较符号
		comp=ParseCompType();
		if(comp==ttUnrecognizedToken)		//未知
		{
			delete left;
			return(nullptr);
		}
		else
		if(comp==ttCloseParanthesis)		//右括号
		{
        	//暂时不支持
			delete left;
			return(nullptr);
		}

		//解析比较式右边
		right=ParseValue();

		if(!right)
		{
			delete left;
			return(nullptr);
		}

		GetToken(ttCloseParanthesis,name);		// )

		//创建比较指令
		{
			DevilCompInterface *dci=nullptr;

			#define DEVIL_COMP_FLAG(flag,func,_lt,_rt)	case flag:dci=new func<_lt,_rt>(left,right);break;

			#define DEVIL_COMP_CREATE(lt,_lt,rt,_rt)	if((left->type==lt)&&(right->type==rt))	\
															switch(comp)	\
															{	\
																DEVIL_COMP_FLAG(ttEqual				,DevilCompEqu		,_lt,_rt)	\
																DEVIL_COMP_FLAG(ttNotEqual			,DevilCompNotEqu	,_lt,_rt)	\
																DEVIL_COMP_FLAG(ttLessThan			,DevilCompLess		,_lt,_rt)	\
																DEVIL_COMP_FLAG(ttGreaterThan		,DevilCompGreater	,_lt,_rt)	\
																DEVIL_COMP_FLAG(ttLessThanOrEqual	,DevilCompLessEqu	,_lt,_rt)	\
																DEVIL_COMP_FLAG(ttGreaterThanOrEqual,DevilCompGreaterEqu,_lt,_rt)	\
															};

			#define DEVIL_COMP_ARRAY(lt,_lt)	DEVIL_COMP_CREATE(lt,_lt,ttBool, 	bool);	\
												DEVIL_COMP_CREATE(lt,_lt,ttInt,	 	int);	\
												DEVIL_COMP_CREATE(lt,_lt,ttUInt,	uint);	\
												DEVIL_COMP_CREATE(lt,_lt,ttFloat,	float);	\
												DEVIL_COMP_CREATE(lt,_lt,ttDouble,	double);\
												DEVIL_COMP_CREATE(lt,_lt,ttInt64,	int64);	\
												DEVIL_COMP_CREATE(lt,_lt,ttUInt64,	uint64);\

			DEVIL_COMP_ARRAY(ttBool, 	bool	)
			DEVIL_COMP_ARRAY(ttInt,	 	int		)
			DEVIL_COMP_ARRAY(ttUInt, 	uint	)
			DEVIL_COMP_ARRAY(ttFloat,	float	)
			DEVIL_COMP_ARRAY(ttDouble,	float	)
			DEVIL_COMP_ARRAY(ttInt64,	int64 	)
			DEVIL_COMP_ARRAY(ttUInt64, 	uint64	)

			#undef DEVIL_COMP_ARRAY
			#undef DEVIL_COMP_FLAG
			#undef DEVIL_COMP_CREATE

			return(dci);
		}
	}

	DevilValueInterface *DevilParse::ParseValue()
	{
		int type;
		UTF16String name,temp;

		DevilValueInterface *dcii=nullptr;

		type=GetToken(name);
		if(type==ttIdentifier)		//未知标识符
		{
			type=CheckToken(temp);

			if(type==ttOpenParanthesis)		//函数调用
			{
				//真实函数调用验证
				{
					DevilFuncMap *map_func=module->GetFuncMap(name);

					if(map_func)
					{
						GetToken(ttOpenParanthesis,name);	//取出 (

						#ifdef _DEBUG
						UTF16String intro;

						DevilCommand *cmd=ParseFuncCall(name,map_func,intro);
						#else
						DevilCommand *cmd=ParseFuncCall(map_func);
						#endif//

						if(cmd)
						{
							switch(map_func->result)
							{
								case ttBool:	dcii=new DevilValueFuncMap<bool		>(module,cmd,ttBool		);break;

								case ttInt8:	dcii=new DevilValueFuncMap<int8		>(module,cmd,ttInt8		);break;
								case ttInt16:   dcii=new DevilValueFuncMap<int16	>(module,cmd,ttInt16	);break;
								case ttInt:     dcii=new DevilValueFuncMap<int32	>(module,cmd,ttInt		);break;

								case ttUInt8:	dcii=new DevilValueFuncMap<uint8	>(module,cmd,ttUInt8	);break;
								case ttUInt16:  dcii=new DevilValueFuncMap<uint16	>(module,cmd,ttUInt16	);break;
								case ttUInt:	dcii=new DevilValueFuncMap<uint32	>(module,cmd,ttUInt		);break;

								case ttFloat:   dcii=new DevilValueFuncMap<float	>(module,cmd,ttFloat	);break;

								case ttString:	dcii=new DevilValueFuncMap<u16char *>(module,cmd,ttString	);break;

								default:		LOG_ERROR(u"if中调用的函数返回类型无法支持");break;
							}
						}
						else
							LOG_ERROR(u"if中的真实函数映射没有找到");
					}
				}

				//脚本函数调用验证
/*				{
					DevilFunc *script_func=module->GetScriptFunc(name);

					if(script_func)
					{
						func->command.Add(new DevilScriptFuncCall(module,script_func));
						return(true);
					}
				}

				ErrorHint(u"脚本调用函数没有找到相应的真实函数映射与脚本函数<%s>",name.c_str());*/
			}
			else	//属性映射
			{
				DevilPropertyMap *dpm=module->GetPropertyMap(name);

				if(dpm)
				{
					switch(dpm->type)
					{
						case ttBool:	dcii=new DevilValueProperty<bool	>(module,dpm,ttBool);break;

						case ttInt8:	dcii=new DevilValueProperty<int8	>(module,dpm,ttInt8);break;
						case ttInt16:	dcii=new DevilValueProperty<int16	>(module,dpm,ttInt16);break;
						case ttInt:		dcii=new DevilValueProperty<int32	>(module,dpm,ttInt);break;
						//case ttInt64:	dcii=new DevilValueProperty<int64	>(module,dpm,ttInt64);break;

						case ttUInt8:	dcii=new DevilValueProperty<uint8	>(module,dpm,ttUInt8);break;
						case ttUInt16:	dcii=new DevilValueProperty<uint16	>(module,dpm,ttUInt16);break;
						case ttUInt:    dcii=new DevilValueProperty<uint32	>(module,dpm,ttUInt);break;
						//case ttUInt64:	dcii=new DevilValueProperty<uint64	>(module,dpm,ttUInt64);break;

						case ttFloat:	dcii=new DevilValueProperty<float	>(module,dpm,ttFloat);break;
						//case ttDouble:	dcii=new DevilValueProperty<double	>(module,dpm,ttDouble);break;

						default:LOG_ERROR(u"if 比较指令暂时不支持<"+UTF16String(GetTokenName(dpm->type))+u">类型的数据进行比较");
								return(nullptr);
					}
				}
				else
				{
                	LOG_ERROR(u"没有找到属性映射:"+name);
					return(nullptr);
				}
			}
		}
		else
		if(type==ttTrue||type==ttFalse)	//布尔型
		{
			dcii=new DevilBool(module,name.c_str());
		}
		else
		if(type==ttIntConstant)			//整数
		{
			dcii=new DevilUInteger(module,name.c_str());
		}
		else
		if(type==ttFloatConstant)		//浮点数
		{
			dcii=new DevilFloat(module,name.c_str());
		}
		else
		if(type==ttDoubleConstant)		//浮点数
		{
			dcii=new DevilDouble(module,name.c_str());
		}
		else
		if(type==ttMinus)				// -号
		{
			type=GetToken(name);

			UTF16String str=u'-';
			str+=name;

			if(type==ttIntConstant)			//整数
			{
				dcii=new DevilInteger(module,str.c_str());
			}
			else
			if(type==ttFloatConstant)		//浮点数
			{
				dcii=new DevilFloat(module,str.c_str());
			}
			else
			if(type==ttDoubleConstant)		//浮点数
			{
				dcii=new DevilDouble(module,str.c_str());
			}
		}

		return(dcii);
	}

	eTokenType DevilParse::ParseCompType()
	{
		eTokenType type;
		UTF16String name;

		type=GetToken(name);

		if(type>=ttEqual
		 &&type<=ttGreaterThanOrEqual)
			return(type);
		else
		if(type==ttCloseParanthesis)
			return(ttCloseParanthesis);
		else
			return(ttUnrecognizedToken);
	}
}//namespace hgl
