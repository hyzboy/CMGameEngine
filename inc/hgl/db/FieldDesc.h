#ifndef HGL_DB_FIELD_DESC_INCLUDE
#define HGL_DB_FIELD_DESC_INCLUDE

#include<hgl/db/FieldType.h>
namespace hgl
{
	namespace db
	{
		/**
		* 字段描述
		*/
		struct FieldDesc
		{
			UTF8String name;																		///<字段名称
			FieldType type;																			///<字段类型
			bool primary;																			///<是否主键
			bool index;																				///<是否是索引
			bool unique;																			///<是否唯一索引
			bool not_null;																			///<是否不可为空
			UTF8String def_value;																	///<缺省值
			bool auto_inc;																			///<自动增加

			int32 serial;																			///<序列号

		public:

			FieldDesc()
			{
				primary=false;
				index=false;
				unique=false;
				not_null=false;
				auto_inc=false;
				serial=-1;
			}

			/**
			 * 字段描述构造函数
			 * @param n 字段名称
			 * @param ft 字段类型
			 * @param p 是否主键
			 * @param i 是否索引
			 * @param u 是否唯一索引
			 * @param nn 是否不可为空
			 * @param dv 缺省值字符串
			 * @param ai 是否自动增量
			 */
			FieldDesc(const UTF8String &n,const FieldType &ft,const bool p,const bool i,const bool u,const bool nn,const UTF8String &dv,const bool ai)
			{
				name=n;
				type=ft;
				primary=p;
				index=i;
				unique=u;
				not_null=nn;
				def_value=dv;
				auto_inc=ai;
				serial=-1;
			}
		};//struct FieldDesc
	}//namespace db
}//namespace hgl
#endif//HGL_DB_FIELD_DESC_INCLUDE
