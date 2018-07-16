#include<hgl/type/DataType.h>
#include<hgl/type/BaseString.h>
#include<hgl/FileSystem.h>
#include<hgl/Desktop.h>
#include<shobjidl.h>

namespace hgl
{
    namespace os
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
        * @param sc 快捷方式配置
        * @return 是否成功
        */
        bool CreateShortCut(const ShortCut &sc)
        {
            IShellLinkW   *psl=nullptr;
            IPersistFile  *pPf=nullptr;

            WideString lnk_filename=sc.lnk_filename;

            lnk_filename += L".lnk";

            HRESULT hresult;

            hresult=CoCreateInstance(CLSID_ShellLink,nullptr,CLSCTX_INPROC_SERVER,IID_IShellLinkW,(LPVOID*)&psl);

            if(hresult!=S_OK)
                return(false);

            hresult=psl->QueryInterface(IID_IPersistFile,(LPVOID*)&pPf);
        
            if(hresult!=S_OK)
                return(false);

            if(!sc.work_directory.IsEmpty())
            {
                hresult=psl->SetWorkingDirectory(sc.work_directory.c_str());

                if(hresult!=S_OK)
                    return(false);
            }

            if(!sc.param.IsEmpty())
            {
                hresult=psl->SetArguments(sc.param.c_str());

                if(hresult!=S_OK)
                    return(false);
            }

            hresult=psl->SetPath(sc.filename.c_str());

            if(hresult!=S_OK)
                return(false);

            if(!sc.icon_filename.IsEmpty())
                if(filesystem::FileExist(sc.icon_filename))
                {
                    hresult=psl->SetIconLocation(sc.icon_filename.c_str(),0);

                    if(hresult!=S_OK)
                        return(false);
                }

            if(!sc.descript.IsEmpty())
            {
                hresult=psl->SetDescription(sc.descript.c_str());

                if(hresult!=S_OK)
                    return(false);
            }

            hresult=pPf->Save(lnk_filename.c_str(),TRUE);

            if(hresult!=S_OK)
                return(false);

            if(pPf)
                pPf->Release();

            if(psl)
                psl->Release();

            return true;
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
    }//namespace os
}//namespace hgl
