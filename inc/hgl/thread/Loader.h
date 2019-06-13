#ifndef HGL_LOADER_INCLUDE
#define HGL_LOADER_INCLUDE

#include<hgl.h>
#include<hgl/type/BaseString.h>
#include<hgl/object/EnumObject.h>
#include<hgl/thread/Thread.h>
namespace hgl
{
    class HAC;

    /**
    * 加载器，用于异步加载数据
    */
    class Loader:public EnumObject,public Thread                                                    ///加载器
    {
    public:

        /**
        * 加载器状态枚举
        */
        enum LoaderStatus       //加载器状态
        {
            lsNone=0,           //未使用

            lsLoad,             //正在加载
            lsProc,             //正在处理
            lsProcFinish,       //处理完成
            lsClear,            //清理
            lsClearFinish,      //清理完成

            lsEnd,              //完成
        };

    protected:

        LoaderStatus status;

        void SetStatus(LoaderStatus);

        HAC *loader_hac;
        UTF16String loader_filename;
        void *loader_pointer;
        void *loader_data;
        int loader_size;

    protected:  //被动事件

        virtual void ProcFinish()=default;                                                          ///<加载完成(异步事件)
        virtual void ProcEnd()=default;                                                             ///<结束处理
        virtual void ProcClear()=default;                                                           ///<清理处理(异步事件)

    public: //事件

        DefEvent(void,OnError,(Loader *));                                                          ///<出错事件
        DefEvent(void,OnEnd,(Loader *));                                                            ///<完成事件,在ProcEnd之后,ProcClear之前被调用

    public:

        Loader();
        virtual ~Loader()=default;

        bool Execute();

        void Update();

    public: //方法

        bool Load(HAC *,const u16char *);                                                           ///<加载一个文件到某个对象

        LoaderStatus GetStatus();                                                                   ///<取得状态
    };//class Loader
}//namespace hgl
#endif//HGL_LOADER_INCLUDE
