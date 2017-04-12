#ifndef HGL_OTHER_INCLUDE
#define HGL_OTHER_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/Str.h>

namespace hgl
{
    //此代码取自xiph.org的opus dec
    static inline uint32 FastRand32()
    {
        static uint32 rngseed = 22222;

        rngseed = (rngseed * 96314165) + 907633515;
        return rngseed;
    }
}//namespace hgl

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
	int GetTimeZone();																				///<返回时区的时差(单位：秒)

	uint64 GetMilliStartTime();																		///<取得毫秒程序启动时间(单位：1/1000秒)
	uint64 GetMicroStartTime();																		///<取得微秒程序启动时间(单位：1/1000000秒)
	double GetDoubleStartTime();																	///<取得秒程序启动时间(单位：秒)

	uint64 GetTime();																				///<取得当前时间(单位：1/1000秒)
	uint64 GetMicroTime();																			///<取得当前时间(单位：1/1000000秒)
	double GetDoubleTime();																			///<取得当前时间(单位：秒)

	double GetLocalDoubleTime();																	///<取得本地当前时间(单位：秒)

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
		void PopupWebBrowser(const u16char *);															///<弹出网页浏览器
		void PopupEmailClient(const u16char *,const u16char *sub=nullptr);							///<弹出电子邮件客户端

		bool CreateShortCut(const u16char *lnk_fname,const u16char *filename,
							const u16char *work_directory,const u16char *param,
							const u16char *icon=nullptr);												///<创建快捷方式

//        void *GetFileIcon(const u16char *,int &,int &,int &);										///<取得文件的系统图标(仅Vista以上可用)
#endif//HGL_OS == HGL_OS_Windows

#if (HGL_OS==HGL_OS_Windows)||(HGL_OS==HGL_OS_MacOS)
		void CopyTextToClipboard(const u16char *);													///<复制字符串到剪贴板
		const u16char *GetTextFromClipboard();														///<从剪贴板取得字符串
#endif//(HGL_OS==HGL_OS_Windows)||(HGL_OS==HGL_OS_MacOS)
	}//namespace os

	using namespace os;
}//namespace hgl
#endif//HGL_OTHER_INCLUDE
