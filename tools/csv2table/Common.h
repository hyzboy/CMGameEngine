#ifndef HGL_CSV2TABLE_COMMON_INCLUDE
#define HGL_CSV2TABLE_COMMON_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/type/StringList.h>
#include<hgl/db/FieldType.h>

using namespace hgl;

int ParseField(UTF16StringList *sl,const UTF16String &str,const u16char ch,bool null_end=false);

inline int ParseFieldName(UTF16StringList *sl,const UTF16String &str)
{
	return ParseField(sl,str,u'\t',true);
}

inline int ParseFieldData(UTF16StringList *sl,const UTF16String &str)
{
	return ParseField(sl,str,u'\t',false);
}

bool WriteFieldData(const db::FieldType &ct,void *buf,UTF16String &str);

#endif//HGL_CSV2TABLE_COMMON_INCLUDE
