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
    void PopupWebBrowser(const wchar_t *url)
    {
        ShellExecuteW(nullptr,nullptr,url,nullptr,nullptr,0);
    }

    /**
    * 弹出邮件客户端软件,并自动指定收件人的email地址。示例: PopupEmailClient(L"hyz@hyzgame.com.cn",L"您好!");
    * @param email 电子邮件地址
    * @param subject 邮件主题
    */
    void PopupEmailClient(const WideString &email,const WideString &subject)
    {
        wchar_t url[MAX_PATH]=L"mailto:";

        strcat(url, MAX_PATH, email);

        const wchar_t subject_header[] = L"?Subject=\"";
        const size_t subject_header_size = sizeof(subject_header)/sizeof(wchar_t);

        strcat(url, MAX_PATH, subject_header, subject_header_size);
        strcat(url, MAX_PATH, subject);
        strcat(url, MAX_PATH, L'\"');

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
    bool CreateShortCut(const wchar_t *lnk_fname,const wchar_t *filename,const wchar_t *work_directory,const wchar_t *param,const wchar_t *icon)
    {
        IShellLinkW   *psl=nullptr;
        IPersistFile  *pPf=nullptr;

        bool result=false;

        WideString lnk_filename=lnk_fname;

        lnk_filename += L".lnk";

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
//        void *GetFileIcon(const wchar_t *filename,int &width,int &height,int &color)
//        {
//            IShellItemImageFactory *pShellItemImageFactory = nullptr;
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
