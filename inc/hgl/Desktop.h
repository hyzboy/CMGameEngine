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

        bool CreateShortCut(const u16char *lnk_fname, const u16char *filename,
                            const u16char *work_directory, const u16char *param,
                            const u16char *icon = nullptr);												///<创建快捷方式

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