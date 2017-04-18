#ifndef HGL_OS_DESKTOP_INCLUDE
#define HGL_OS_DESKTOP_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
    /**
    * 与本地操作系统相关功能函数的名字空间
    */
    namespace os		///操作系统相关功能名字空间
    {
#if HGL_OS == HGL_OS_Windows
        void PopupWebBrowser(const u16char *);															///<弹出网页浏览器
        void PopupEmailClient(const u16char *, const u16char *sub = nullptr);							///<弹出电子邮件客户端
        
        /**
        * 快捷方式数据结构
        */
        struct ShortCut
        {
            OSString lnk_filename;          ///<注意不要带扩展名
            OSString filename;              ///<执行的程序文件全名
            OSString work_directory;        ///<工作目录
            OSString param;                 ///<参数
            OSString icon_filename;         ///<图标文件
            OSString descript;              ///<备注
        };//struct ShortCut

        bool CreateShortCut(const ShortCut &);												        ///<创建快捷方式

//        void *GetFileIcon(const u16char *,int &,int &,int &);										///<取得文件的系统图标(仅Vista以上可用)
#endif//HGL_OS == HGL_OS_Windows

#if (HGL_OS==HGL_OS_Windows)||(HGL_OS==HGL_OS_MacOS)
        void CopyTextToClipboard(const u16char *);													///<复制字符串到剪贴板
        const u16char *GetTextFromClipboard();														///<从剪贴板取得字符串
#endif//(HGL_OS==HGL_OS_Windows)||(HGL_OS==HGL_OS_MacOS)
    }//namespace os

    using namespace os;
}//namespace hgl
#endif//HGL_OS_DESKTOP_INCLUDE