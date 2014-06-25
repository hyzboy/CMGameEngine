#ifndef HGL_QT4_INCLUDE
#define HGL_QT4_INCLUDE

#include<hgl/hgl.h>
#include<hgl/platform/ConsoleSystemInitInfo.h>
#include<hgl/platform/QT4Application.h>

#if HGL_OS == HGL_OS_Windows
	inline QString toQString(const hgl::UTF16String &ws)
	{
		return QString::fromWCharArray(ws.c_str(),ws.Length());
	}

	inline QString ToQString(const wchar_t *str,int size){return QString::fromUtf16((ushort *)str,size);}
#else
	template<int WS> QString WCharToQString(const wchar_t *,int);

	template<> inline QString WCharToQString<2>(const wchar_t *str,int size){return QString::fromUtf16((ushort *)str,size);}
	template<> inline QString WCharToQString<4>(const wchar_t *str,int size){return QString::fromUcs4((uint *)str,size);}

	inline QString toQString(const wchar_t *str,int size)
	{
		return WCharToQString<sizeof(wchar_t)>(str,size);
	}

	inline QString ToQString(const char16_t *str,int size){return QString::fromUtf16((ushort *)str,size);}
	inline QString ToQString(const char32_t *str,int size){return QString::fromUcs4((uint *)str,size);}
#endif//HGL_OS == HGL_OS_Windows

inline QString toQString(const hgl::UTF8String &str)
{
	return QString::fromUtf8(str.c_str(),str.Length());
}

inline QString toQString(const hgl::UTF16String &str)
{
	return QString::fromUtf16((ushort *)str.c_str(),str.Length());
}

// inline QString toQString(const hgl::UTF32String &str)
// {
// 	return QString::fromUcs4((uint *)str.c_str(),str.Length());
// }

#define HGL_QT_MAIN(sii_name,app_name,arg_name)	extern "C" int QTAppMain(hgl::ConsoleSystemInitInfo &sii_name,hgl::QTGuiApplication &app_name,const hgl::StringList<hgl::OSString> &arg_name)

#define HGL_QT_MAIN_FUNC()							HGL_QT_MAIN(sii,app,args)

#define HGL_QT_APPLICATION(name,code,start)	using namespace hgl;	\
													\
													HGL_QT_MAIN_FUNC()\
													{	\
														sii.info.ProjectName=name;	\
														sii.info.ProjectCode=code;	\
														\
														if(!app.Init(&sii))	\
															return(-1);	\
														\
														app.SetStart(start);	\
														\
														return app.Run();	\
													}
#endif//#ifndef HGL_QT4_INCLUDE
