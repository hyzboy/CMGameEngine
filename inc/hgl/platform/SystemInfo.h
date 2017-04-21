#ifndef HGL_SYSTEMINFO_INCLUDE
#define HGL_SYSTEMINFO_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/type/BaseString.h>
namespace hgl
{
    /**
    * 内存信息结构体
    */
    struct MemInfo                      ///内存信息结构体
    {
        uint64  AvailPhys;              ///<有效物理内存
        uint64  TotalPhys;              ///<总共物理内存

        uint64  AvailPageFile;          ///<有效页面文件
        uint64  TotalPageFile;          ///<总共页面文件

        uint64  AvailVirtual;           ///<有效虚拟内存
        uint64  TotalVirtual;           ///<总计虚拟内存

    public:

        MemInfo();
    };//struct MemInfo

    /**
    * 操作系统及《古月》相关路径
    */
    struct CMGDKPATH                    ///操作系统及《古月》相关路径
    {
        OSString start;                 ///<当前应用程序起始路径

        OSString os;                    ///<操作系统路径
        OSString osfont;                ///<操作系统字库路径
        OSString library;               ///<操作系统辅助库路径

        OSString common_data;           ///<所有用户共用应用程序数据路径
        OSString local_data;            ///<本地应用程序数据路径

        OSString temp;                  ///<临时文件路径

        OSString mydata;                ///<当前用户应用程序数据保存路径
        OSString myprogram;             ///<“我的程序”路径(Windows下为开始菜单程序路径,Mac下为应用程序路径)
        OSString mydesktop;             ///<“桌面”路径
        
        OSString desktop;               ///<所有用户桌面路径
        
        OSString cm;                    ///<《古月》路径

        OSString gui;                   ///<《古月》GUI资源路径
        OSString plug_ins;              ///<《古月》插件路径

    public:

        CMGDKPATH();
    };//struct CMGDKPATH

    /**
    * 系统信息
    */
    struct SystemInfo                   ///系统信息结构
    {
        MemInfo     mem;                ///<内存信息

        u16char     os_name[256];       ///<操作系统全名

        CMGDKPATH   path;               ///<《古月》相关路径

    public:

        SystemInfo();
    };//struct SystemInfo
}//namespace hgl
#endif//HGL_SYSTEMINFO_INCLUDE
