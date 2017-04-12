#ifndef HGL_EXPEND_STRING_INCLUDE
#define HGL_EXPEND_STRING_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/Str.h>

namespace hgl   //其它
{
	void SetExpendString(const UTF16String &,const UTF16String &);									///<设置特殊扩展字符串
	void ClearExpendString(const UTF16String &);													///<清除特殊扩展字符串
	bool GetExpendString(const UTF16String &,UTF16String &);										///<取得特殊扩展字符串

	bool ConvertExpendString(const UTF16String &,UTF16String &);									///<转换特殊扩展字符串
}//namespace hgl
#endif//HGL_EXPEND_STRING_INCLUDE
