#ifndef HGL_PLUGIN_INCLUDE
#define HGL_PLUGIN_INCLUDE
//--------------------------------------------------------------------------------------------------
#include<hgl/type/DataType.h>
namespace hgl
{
	/**
	* 插件类型枚举
	*/
	enum PlugInType			///插件类型枚举
	{
		pitNormal=0,		///<普通插件

		pitLog,				///<日志,输出到文件
		pitPlatform,		///<平台
		pitOpenGL,			///<OpenGL
		pitOpenAL,			///<OpenAL

		pitEnd
	};

	typedef void 		(*InitPlugInPROC)();														//初始化插件
	typedef void 		(*ClosePlugInPROC)();														//关闭插件

	typedef uint32		(*GetPlugInVersionPROC)();					                				//取得插件版本号
	typedef u16char *	(*GetPlugInIntroPROC)();				                    				//取得插件介绍信息

	typedef bool		(*GetPlugInInterfacePROC)(uint32,void *);			        				//取得插件接口
	typedef void		(*SetPlugInInterfacePROC)(void *,void *);	                				//设置插件接口

	typedef bool		(*LoadInterfaceByNamePROC)(const u16char *,uint32,void *);					//取得接口
	typedef bool		(*LoadInterfaceByIndexPROC)(uint32,uint32,void *);							//取得接口
}//namespace hgl

#ifndef __MAKE_PLUGIN__
#include<hgl/type/BaseString.h>
#include<hgl/type/Map.h>
#include<hgl/ExternalModule.h>
#include<hgl/LogInfo.h>
#include<string.h>
namespace hgl
{
	/**
	* 插件基类
	*/
	class PlugIn																					///插件
	{
	protected:

		int ref_count;

		uint ver;
		u16char *intro;

		PlugInType type;																			///<插件类型

		OSString name;																				///<插件名称
		OSString filename;																			///<插件文件名

	public:

		const PlugInType &	GetType		()const{return type;}										///<取得插件类型
		const uint			GetVersion	()const{return ver;}										///<取得插件版本
		const u16char *	GetIntro	()const{return intro;}										///<取得插件介绍
		const OSString &	GetName		()const{return name;}										///<取得插件名称

	public:

		PlugIn();
		virtual ~PlugIn();

		virtual void AddRef(){ref_count++;}
		virtual int Release(){return(--ref_count);}

		GetPlugInInterfacePROC GetInterface;                                             			///<取得接口函数指针
	};//class PlugIn

	/**
	* 外部插件
	*/
	class ExternalPlugIn:public PlugIn                                                              ///外部插件
	{
	protected:

		ExternalModule module;                                                                      ///<插件模块

		bool Load();

	public:

		ExternalPlugIn(const OSString &,const PlugInType &pit=pitNormal);
		virtual ~ExternalPlugIn();
	};//class ExternalPlugIn

	void		InitPlugIn();																		///<初始化插件管理
	void		ClearAllPlugIn();																	///<清除所有插件

	void		SetPlugIn(PlugIn *,PlugInType);														///<设置一个插件

	PlugIn * 	LoadPlugIn(const OSString &,PlugInType=pitNormal);									///<加载一个插件
	void 		UnloadPlugIn(PlugIn *);																///<释放一个插件

	void * 		LoadInterface(const OSString &,int);												///<加载一个插件，并取得指定接口
	//--------------------------------------------------------------------------------------------------
	template<typename T> struct PlugInFlagData:public Pair<OSString,T>
	{
		PlugIn *pi;

	public:

		PlugInFlagData():Pair<OSString,T>()
		{
			pi=nullptr;
		}

		~PlugInFlagData()
		{
		}
	};//template<typename T> struct PlugInFlagData

	template<typename T>
	T *CheckPlugIn(_MapObject<OSString,T,PlugInFlagData<T *> > &PIM,const OSString &front,uint ver,const OSString &name)
	{
		T *iface;

		if(PIM.Get(name,iface))					//接口可能本身就为NULL,所以不能直接用PIM[name]
			return(iface);

		PlugIn *pi=LoadPlugIn(front+OS_TEXT(".")+name);	//加载插件

		if(pi)	//加载插件成功
		{
			if(pi->GetVersion()<ver)
			{
				LOG_ERROR(OS_TEXT("插件<")+name+OS_TEXT(">的版本太低！"));
			}
			else
			{
				iface=new T;

				if(pi->GetInterface(ver,iface))
				{
					PIM.Add(name,iface)->pi=pi;

					return(iface);
				}

				delete iface;

				LOG_ERROR(OS_TEXT("获取插件<")+name+OS_TEXT(">接口<ver:")+OSString(ver)+OS_TEXT(">失败!"));
			}
		}

		PIM.Add(name,nullptr);

		if(pi)
			UnloadPlugIn(pi);

		LOG_ERROR(OS_TEXT("加载插件<")+name+OS_TEXT("失败!"));
		return(nullptr);
	}

	void RegistryPIMClear(void (*)());		//注册清除函数

	#define	PlugInManage(name,str_name,ver) static CusMapObject<OSString,name##PlugInInterface,PlugInFlagData<name##PlugInInterface *> > name##Interface;	\
											\
											void name##PlugInClear()	\
											{	\
												int n=name##Interface.GetCount();	\
												\
												if(!n)return;	\
												\
												LOG_INFO(OS_TEXT("Clear<") OS_TEXT(str_name) OS_TEXT("> Plug-Ins,number=")+OSString(n)+OS_TEXT("."));	\
												\
												while(n--)	\
												{	\
													PlugInFlagData<name##PlugInInterface *> *pifd=name##Interface.GetItem(n);	\
													\
													if(pifd)	\
														UnloadPlugIn(pifd->pi);	\
												}	\
												\
												name##Interface.Clear();	\
											}	\
											\
											name##PlugInInterface *name##InterfaceCheck(const OSString &plugin_name)	\
											{	\
												static bool first=true;	\
												\
												if(first)	\
												{	\
													first=false;	\
													\
													LOG_INFO(OS_TEXT("Registry <") OS_TEXT(str_name) OS_TEXT("> Plug-In Manager."));	\
													RegistryPIMClear(name##PlugInClear);	\
												}	\
												\
												return CheckPlugIn<name##PlugInInterface>(name##Interface,OS_TEXT(str_name),ver,plugin_name);	\
											}
}//namespace hgl
#endif//__MAKE_PLUGIN__
#endif//HGL_PLUGIN_INCLUDE
