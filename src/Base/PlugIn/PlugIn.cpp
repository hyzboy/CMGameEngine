#include<hgl/type/List.h>
#include<hgl/Info.h>
#include<hgl/FileSystem.h>
#include<hgl/PlugIn.h>

namespace hgl
{
    using PIMClearFunc=void (*)();

    static MapObject<OSString,PlugIn>    plugin_module;                                              ///<插件模块列表
    static PlugIn *                      SpecialPlugIn[pitEnd-1];                                    ///<特殊插件
    static List<PIMClearFunc>            plugin_manager_list;                                        ///<插件管理器清除函数列表

    /**
    * 加载一个插件
    * @param plugin_name 插件名称
    * @param type 特殊插件类型
    * @return 返回模块指针,返回NULL表示失败
    */
    PlugIn *LoadPlugIn(const OSString &plugin_name,PlugInType type)
    {
        PlugIn *pi=plugin_module[plugin_name];

        if(pi)
        {
            pi->AddRef();                        //增加一个引用计数
            return pi;
        }

        pi=new ExternalPlugIn(HGL_PLUGIN_FRONTNAME+plugin_name,type);

        if(pi->GetInterface==nullptr
         &&pi->GetVersion()==0
         &&pi->GetIntro()==nullptr)
        {
            delete pi;
            return(nullptr);
        }

        plugin_module.Add(plugin_name,pi);

        if(type>pitNormal&&type<pitEnd)
            SpecialPlugIn[type-1]=pi;

        return(pi);
    }

    /**
    * 释放一个插件
    * @param pi 插件指针
    */
    void UnloadPlugIn(PlugIn *pi)
    {
        if(!pi)return;

        if(pi->Release()>0)            //还有人在使用
            return;

        if(pi->GetType()!=pitNormal)
            SpecialPlugIn[pi->GetType()-1]=nullptr;

        plugin_module.DeleteByValue(pi);
    }

    void RegistryPIMClear(void (*func)())
    {
        plugin_manager_list+=func;
    }

    void InitPlugIn()
    {
        hgl_zero(SpecialPlugIn);
    }

    void ClearAllPlugIn()
    {
        int n=plugin_manager_list.GetCount();

        while(n--)
            plugin_manager_list[n]();

        plugin_manager_list.Clear();
        plugin_module.Clear();
    }
}

namespace hgl
{
    /**
    * 设置插件
    * @param pi 插件指针
    * @param type 插件类型
    */
    void SetPlugIn(PlugIn *pi,PlugInType type)
    {
        if(type>pitNormal&&type<pitEnd)
            SpecialPlugIn[type-1]=pi;

        if(!plugin_module.ValueExist(pi))
            plugin_module.Add(pi->GetName(),pi);
    }

    bool LoadInterfaceByName(const OSString &name,int ver,void *data)
    {
        PlugIn *pi=LoadPlugIn(name);

        if(pi)
            return(pi->GetInterface(ver,data));
        else
            return(false);
    }

    bool LoadInterfaceByIndex(int index,int ver,void *data)
    {
        if(index>pitNormal&&index<pitEnd)
        {
            PlugIn *pi=SpecialPlugIn[index-1];

            if(pi)
                return pi->GetInterface(ver,data);
        }

        return(false);
    }

    bool LoadInterface(const OSString &name,int ver,void *data)
    {
        return LoadInterfaceByName(name,ver,data);
    }
}

namespace hgl
{
    PlugIn::PlugIn()
    {
        ref_count=1;

        ver=0;
        intro=nullptr;

        GetInterface=nullptr;
    }

    PlugIn::~PlugIn()
    {
        LOG_INFO(OS_TEXT("release Plug-in <")+name+OS_TEXT(">:<")+filename+OS_TEXT("> ok"));
    }
}

namespace hgl
{
    ExternalPlugIn::ExternalPlugIn(const OSString &plugin_name,const PlugInType &pit)
    {
        {
            const OSString &pi_path=GetString(hfsPlugInPath);

            filename=filesystem::MergeFilename(pi_path,plugin_name);

            filename+=HGL_PLUGIN_EXTNAME;
        }

        type=pit;
        name=plugin_name;

        ver=0;
        intro=nullptr;
        GetInterface=nullptr;

        if(module.Load(filename.c_str()))
            if(Load())
            {
                #ifdef _DEBUG
                    #if HGL_OS == HGL_OS_Windows
                        LOG_INFO(OS_TEXT("Load Plug-in <") + plugin_name + OS_TEXT(">:<") + filename + OS_TEXT("> ok,Plug-In intro:") + OSString(intro));
                    #else
                        LOG_INFO(OS_TEXT("Load Plug-in <") + plugin_name + OS_TEXT(">:<") + filename + OS_TEXT("> ok,Plug-In intro:") + to_oschar(intro));
                    #endif//windows
                #else
                    LOG_INFO(OS_TEXT("Load Plug-in <")+plugin_name+OS_TEXT(">:<")+filename+OS_TEXT("> ok"));
                #endif//_DEBUG

                InitPlugInPROC InitPlugIn;

                InitPlugIn=(InitPlugInPROC)module.FindFunc("InitPlugIn");

                if(InitPlugIn)
                    InitPlugIn();

                return;
            }

        LOG_INFO(OS_TEXT("Load Plug-in <")+plugin_name+OS_TEXT(">:<")+filename+OS_TEXT("> error!"));
    }

    ExternalPlugIn::~ExternalPlugIn()
    {
        ClosePlugInPROC ClosePlugIn;

        ClosePlugIn=(ClosePlugInPROC)module.FindFunc("ClosePlugIn");

        if(ClosePlugIn)
            ClosePlugIn();
    }

    bool ExternalPlugIn::Load()
    {
        {
            GetPlugInVersionPROC getver;

            getver=(GetPlugInVersionPROC)module.FindFunc("GetPlugInVersion");

            if(!getver)
            {
                LOG_INFO(OS_TEXT("Don't in Plug-in <")+filename+OS_TEXT("> find \"GetPlugInVersion\" "));
                return(false);
            }

            ver=getver();
        }

        {
            GetPlugInIntroPROC getintro;

            getintro=(GetPlugInIntroPROC)module.FindFunc("GetPlugInIntro");

            if(!getintro)
            {
                LOG_INFO(OS_TEXT("Don't in Plug-in <")+filename+OS_TEXT("> find \"GetPlugInIntro\""));
                return(false);
            }

            intro=getintro();
        }

        {
            GetInterface=(GetPlugInInterfacePROC)module.FindFunc("GetPlugInInterface");

            {
                SetPlugInInterfacePROC SetInterface=(SetPlugInInterfacePROC)module.FindFunc("SetPlugInInterface");

                if(SetInterface)
                    SetInterface((void *)LoadInterfaceByName,(void *)LoadInterfaceByIndex);
            }
        }

        return(true);
    }
}//namespace hgl
