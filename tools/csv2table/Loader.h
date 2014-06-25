#ifndef HGL_CSV2TABLE_LOADER_INCLUDE
#define HGL_CSV2TABLE_LOADER_INCLUDE

#include<hgl/type/StringList.h>
#include<hgl/db/FieldType.h>

using namespace hgl;
using namespace hgl::db;

#define UNREAD_LINE_FRONT	';'

/**
 * 各种数据加载的基类
 */
class Loader
{
public:

	virtual ~Loader(){}

public:

	virtual 		bool			Load			(const OSString &,const CharSet &)=0;			///<加载文件

	virtual const	UTF16StringList &GetFieldNameList()=0;											///<取得字段名称列表
	virtual const	FieldTypeList &	GetFieldTypeList()=0;											///<取得字段类型列表
	virtual const	UTF16StringList &GetFieldTypeNameList()=0;										///<取得字段类型名称列表

	virtual			int				GetFieldCount	()=0;											///<取得字段数量
	virtual 		int				GetRecordCount	()=0;											///<取得数据数量
	virtual 		UTF16StringList *GetRecord		(int row)=0;									///<取得字段数据
	virtual 		void			Release			(UTF16StringList *)=0;
};//class Loader

#endif//HGL_CSV2TABLE_LOADER_INCLUDE
