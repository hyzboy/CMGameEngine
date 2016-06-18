#ifndef HGL_QT_STRING_INCLUDE
#define HGL_QT_STRING_INCLUDE

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

	inline QString toQString(const u16char *str,int size){return QString::fromUtf16((ushort *)str,size);}
	inline QString toQString(const char32_t *str,int size){return QString::fromUcs4((uint *)str,size);}

	inline QString toQString(const hgl::UTF16String &str)
	{
		return QString::fromUtf16((ushort *)str.c_str(), str.Length());
	}
#endif//HGL_OS == HGL_OS_Windows

inline QString toQString(const hgl::UTF8String &str)
{
	return QString::fromUtf8(str.c_str(),str.Length());
}

// inline QString toQString(const hgl::UTF32String &str)
// {
// 	return QString::fromUcs4((uint *)str.c_str(),str.Length());
// }
#endif//HGL_QT_STRING_INCLUDE
