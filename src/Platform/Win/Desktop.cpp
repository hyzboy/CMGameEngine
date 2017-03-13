#include<hgl/Other.h>
#include<hgl/type/DataType.h>
#include<hgl/type/BaseString.h>

#include<windows.h>
#include<shobjidl.h>

namespace hgl
{
    /**
    * 弹出一个网页浏览器,并自动打开指定的网址。示例: PopupWebBrowser(L"http://www.hyzgame.com.cn");
    * @param url 网址
    */
    void PopupWebBrowser(const u16char *url)
    {
        ShellExecuteW(nullptr,nullptr,url,nullptr,nullptr,0);
    }

    /**
    * 弹出邮件客户端软件,并自动指定收件人的email地址。示例: PopupEmailClient(L"hyz@hyzgame.com.cn",L"您好!");
    * @param email 电子邮件地址
    * @param subject 邮件主题
    */
    void PopupEmailClient(const OSString &email,const OSString &subject)
    {
        u16char url[4096]=U16_TEXT("mailto:");

        strcat(url, 4096, email);

        const u16char subject_header[] = U16_TEXT("?Subject=\"");
        const size_t subject_header_size = sizeof(subject_header)/sizeof(u16char);

        strcat(url, 4096, subject_header, subject_header_size);
        strcat(url, 4096, subject);
        strcat(url, 4096, U16_TEXT('\"'));

        ShellExecuteW(nullptr,nullptr,url,nullptr,nullptr,0);
    }

    /**
    * 创建快捷方式
    * @param lnk_fname 快捷方式文件名
    * @param filename 要创建快捷方式的文件
    * @param work_directory 工作目录
    * @param param 启动参数
    * @param icon 图标文件
    * @return 是否成功
    */
    bool CreateShortCut(const u16char *lnk_fname,const u16char *filename,const u16char *work_directory,const u16char *param,const u16char *icon)
    {
        IShellLinkW   *psl   =   NULL;
        IPersistFile  *pPf   =   NULL;

        bool result=false;

        UTF16String lnk_filename=lnk_fname;

        lnk_filename += U16_TEXT(".lnk");

        if(CoCreateInstance(CLSID_ShellLink,nullptr,CLSCTX_INPROC_SERVER,IID_IShellLinkW,(LPVOID*)&psl)==S_OK)
        if(psl->QueryInterface(IID_IPersistFile,(LPVOID*)&pPf)==S_OK)
        if(!work_directory||psl->SetWorkingDirectory(work_directory)==S_OK)
        if(!param||strlen(param)<=0||psl->SetArguments(param)==S_OK)
        if(psl->SetPath(filename)==S_OK)
        {
            if(strlen(icon)>0)
                if(FileConfirm(icon))
                    psl->SetIconLocation(icon,0);

            if(pPf->Save(lnk_filename,TRUE)==S_OK)
                result=true;
        }

        if(pPf)
            pPf->Release();

        if(psl)
            psl->Release();

        return result;
    }

//#if (NTDDI_VERSION >= NTDDI_VISTA)
//        /**
//        * 取得文件图标
//        * @param filename 文件名
//        * @param width 图标宽度
//        * @param height 图标高度
//        * @param color 图标色彩数
//        * @return 图标象素数据(请自行delete[],另返回NULL表示失败)
//        */
//        void *GetFileIcon(const u16char *filename,int &width,int &height,int &color)
//        {
//            IShellItemImageFactory *pShellItemImageFactory = NULL;
//            IBindCtx *m_pBindContext;
//
//            //该函数要求ie7.0，但无法确定
//            if(SHCreateItemFromParsingName(filename
//                    m_pBindContext,
//                    IID_PPV_ARGS(&pShellItemImageFactory)
//                    )!=S_OK)return(false);
//        }
//#endif//NTDDI_VERSION >= NTDDI_VISTA
}//namespace hgl
