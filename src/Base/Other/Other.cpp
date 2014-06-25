#include<hgl/Other.h>
#include<hgl/type/DataType.h>
#include<hgl/type/BaseString.h>

#if HGL_OS == HGL_OS_Windows
#include<windows.h>
#include<shobjidl.h>
#endif//HGL_OS == HGL_OS_Windows

namespace hgl
{
	namespace os
	{
#if HGL_OS == HGL_OS_Windows
        /**
		* 弹出一个网页浏览器,并自动打开指定的网址。示例: PopupWebBrowser(u"http://www.hyzgame.com.cn");
        * @param url 网址
        */
        void PopupWebBrowser(const char16_t *url)
        {
            ShellExecute(nullptr,nullptr,url,nullptr,nullptr,nullptr);
        }

        /**
        * 弹出邮件客户端软件,并自动指定收件人的email地址。示例: PopupEmailClient(u"hyz@hyzgame.com.cn",u"您好!");
        * @param email 电子邮件地址
        * @param subject 邮件主题
        */
        void PopupEmailClient(const char16_t *email,const char16_t *subject)
        {
            char16_t url[4096]=u"mailto:";

			strcat(url,email);

			strcat(url,u"?Subject=\"");
			strcat(url,subject);
			strcat(url,u"\"");

            ShellExecute(nullptr,nullptr,url,nullptr,nullptr,nullptr);
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
		bool CreateShortCut(const char16_t *lnk_fname,const char16_t *filename,const char16_t *work_directory,const char16_t *param,const char16_t *icon)
		{
			IShellLinkW   *psl   =   NULL;
			IPersistFile  *pPf   =   NULL;

			bool result=false;

			UTF16String lnk_filename=lnk_fname;

            lnk_filename+=u".lnk";

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
//		/**
//		* 取得文件图标
//		* @param filename 文件名
//		* @param width 图标宽度
//		* @param height 图标高度
//		* @param color 图标色彩数
//		* @return 图标象素数据(请自行delete[],另返回NULL表示失败)
//		*/
//		void *GetFileIcon(const char16_t *filename,int &width,int &height,int &color)
//		{
//			IShellItemImageFactory *pShellItemImageFactory = NULL;
//    		IBindCtx *m_pBindContext;
//
//			//该函数要求ie7.0，但无法确定
//			if(SHCreateItemFromParsingName(filename
//					m_pBindContext,
//					IID_PPV_ARGS(&pShellItemImageFactory)
//                    )!=S_OK)return(false);
//		}
//#endif//NTDDI_VERSION >= NTDDI_VISTA
#endif//HGL_OS == HGL_OS_Windows
	}//namespace os
}//namespace hgl
