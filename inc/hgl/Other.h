#ifndef HGL_OTHER_INCLUDE
#define HGL_OTHER_INCLUDE

#include<hgl/File.h>
#include<hgl/type/DataType.h>
#include<hgl/Str.h>

namespace hgl   //校验/加密算法
{
	void OverflowEncrypt(void *,void *,int,void *,int);			//溢出加密(轻度加密)
	void OverflowDecrypt(void *,void *,int,void *,int);			//溢出解密
}//namespace hgl

namespace hgl   //排序,查找
{
//	int DichFind(void *,int,int,void *);                                                            //2分查找
//	int DichFind(void *,int,int,void *,Sort *);                                                   	//2分查找
}//namespace hgl

namespace hgl   //时间
{
	uint64 GetStartTime();																			///<取得程序启动时间(单位：1/1000秒)
	uint64 GetMicroStartTime();																		///<取得程序启动时间(单位：1/1000000秒)
	double GetDoubleStartTime();																	///<取得程序启动时间(单位：秒)

	uint64 GetTime();																				///<取得当前时间(单位：1/1000秒)
	uint64 GetMicroTime();																			///<取得当前时间(单位：1/1000000秒)
	double GetDoubleTime();																			///<取得当前时间(单位：秒)

	void WaitTime(double);																			///<等待一定时间(单位：秒)
}//namespace hgl

namespace hgl   //其它
{
	void SetExpendString(const UTF16String &,const UTF16String &);									///<设置特殊扩展字符串
	void ClearExpendString(const UTF16String &);													///<清除特殊扩展字符串
	bool GetExpendString(const UTF16String &,UTF16String &);										///<取得特殊扩展字符串

	bool ConvertExpendString(const UTF16String &,UTF16String &);									///<转换特殊扩展字符串

	/**
	* 与本地操作系统相关功能函数的名字空间
	*/
	namespace os		///操作系统相关功能名字空间
	{
#if HGL_OS == HGL_OS_Windows
		void PopupWebBrowser(const char16_t *);															///<弹出网页浏览器
		void PopupEmailClient(const char16_t *,const char16_t *sub=nullptr);							///<弹出电子邮件客户端

		bool CreateShortCut(const char16_t *lnk_fname,const char16_t *filename,
							const char16_t *work_directory,const char16_t *param,
							const char16_t *icon=nullptr);												///<创建快捷方式

//        void *GetFileIcon(const char16_t *,int &,int &,int &);										///<取得文件的系统图标(仅Vista以上可用)
#endif//HGL_OS == HGL_OS_Windows

#if (HGL_OS==HGL_OS_Windows)||(HGL_OS==HGL_OS_MacOS)
		void CopyTextToClipboard(const char16_t *);													///<复制字符串到剪贴板
		const char16_t *GetTextFromClipboard();														///<从剪贴板取得字符串
#endif//(HGL_OS==HGL_OS_Windows)||(HGL_OS==HGL_OS_MacOS)
	}//namespace os

	using namespace os;
}//namespace hgl
#endif//HGL_OTHER_INCLUDE
