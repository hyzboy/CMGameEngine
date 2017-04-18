#ifndef HGL_SYSTEMINFO_INCLUDE
#define HGL_SYSTEMINFO_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/type/BaseString.h>
namespace hgl
{
    /**
    * CPU信息结构体
    */
    struct CpuInfo                  ///CPU信息结构体
    {
        char    Vendor[64];         ///<生产商
        char    Brand[64];          ///<名称

        int        Family;             ///<家族编号
        int        Model;                ///<型号
        int        SteppingID;            ///<步进

        int        Type;                ///<类型
        int        ExtModel;            ///<扩展型号
        int        ExtFamily;            ///<扩展家族

        uint    BrandID;
        uint    CLFLUSHcache;        ///<CLFLUSH Cache容量
        uint    LogicCpuCount;        ///<逻辑处理器数量
        uint    APIC_ID;            ///<APIC物理ID

        bool    SSE3Instructions;
        bool    MONITOR_MWAIT;
        bool    CPLQualifiedDebugStore;
        bool    VirtualMachineExtensions;
        bool    EnhancedIntelSpeedStepTechnology;
        bool    ThermalMonitor2;
        bool    SupplementalSSE3;
        bool    L1ContextID;
        bool    CMPXCHG16B;
        bool    xTPRUpdateControl;
        bool    PerfDebugCapabilityMSR;
        bool    SSE41Extensions;
        bool    SSE42Extensions;
        bool    POPCNT;
        bool    MultiThreading;

        uint    CacheLineSize;        ///<
        uint    CacheSize;            ///<缓存容量
        //uint    CacheL2Size;        ///<2级缓存容量

        uint    PhysicalAddress;
        uint    VirtualAddress;

        bool    NestedPaging;
        bool    LBRVisualization;

        bool    FP128;
        bool    MOVOptimization;

        bool    intel_ext[4][32];    ///<Intel扩展
        bool    amd_ext[4][32];        ///<AMD扩展

    public:

        CpuInfo();

        bool    __inline check_mmx()        {return intel_ext[3][23];}
        bool    __inline check_sse()        {return intel_ext[3][25];}
        bool    __inline check_sse2()       {return intel_ext[3][26];}
        bool    __inline check_htt()        {return intel_ext[3][28];}
        bool    __inline check_ia64()       {return intel_ext[3][30];}
        bool    __inline check_sse3()       {return intel_ext[2][0];}
        bool    __inline check_ssse3()      {return intel_ext[2][9];}
        bool    __inline check_sse41()      {return intel_ext[2][19];}
        bool    __inline check_sse42()      {return intel_ext[2][20];}

        bool    __inline check_3dnow()      {return amd_ext[3][31];}
        bool    __inline check_3dnow2()     {return amd_ext[3][30];}
        bool    __inline check_amd64()      {return amd_ext[3][29];}
        bool    __inline check_sse5()       {return amd_ext[2][11];}
        bool    __inline check_sse4a()      {return amd_ext[2][6];}
    };//struct CpuInfo

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
        CpuInfo     cpu;                ///<CPU信息
        MemInfo     mem;                ///<内存信息

        u16char     os_name[256];       ///<操作系统全名

        CMGDKPATH   path;               ///<《古月》相关路径

    public:

        SystemInfo();
    };//struct SystemInfo
}//namespace hgl
#endif//HGL_SYSTEMINFO_INCLUDE
