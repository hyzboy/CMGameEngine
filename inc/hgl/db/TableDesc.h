#ifndef HGL_DB_TABLE_DESC_INCLUDE
#define HGL_DB_TABLE_DESC_INCLUDE

#include<hgl/db/FieldDesc.h>
#include<hgl/type/Map.h>
namespace hgl
{
	namespace db
	{
		/**
		* 表结构描述
		*/
		class TableDesc
		{
			UTF8String table_name;																	///<表名

			ObjectList<FieldDesc> field_list;														///<字段列表
			FieldDesc *primary_field;																///<主字段

			Map<UTF8String,FieldDesc *> field_by_name;										///<字段列表

			Map<UTF8String,UTF8String> extra_attrib;											///<额外属性

		public:

			TableDesc(const UTF8String &n)
			{
				table_name=n;
				primary_field=nullptr;
			}

			~TableDesc()=default;

			bool AddField(FieldDesc *);																///<增加一个字段

		public:

			const UTF8String &	GetTableName()const{return table_name;}								///<取得表名
			const int			GetFieldCount()const{return field_list.GetCount();}					///<取得字段数量
			const FieldDesc *	GetPrimaryField()const{return primary_field;}						///<取得主字段

			const FieldDesc *	GetField(const int index)const{return field_list[index];}			///<按序列号取得字段
			const FieldDesc *	GetField(const UTF8String &fn)const									///<按名字取得字段
			{
				FieldDesc *fd;

				if(field_by_name.Get(fn,fd))
					return fd;

				return(nullptr);
			}

			const void			AddExtraAttrib(const UTF8String &name,const UTF8String &attrib)		///<增加一个额外属性
			{
				extra_attrib.Add(name,attrib);
			}

			const bool			GetExtraAttrib(const UTF8String &name,UTF8String &attrib)const		///<取得额外属性
			{
				return extra_attrib.Get(name,attrib);
			}
		};//class TableDesc

		typedef MapObject<UTF8String,TableDesc> TableDescList;									///<表描述列表

		bool LoadTableDesc(TableDescList &table_desc_list,char *,int);								///<从XML数据中加载数据表结构描述列表
		bool LoadTableDesc(TableDescList &table_desc_list,const OSString &filename);				///<从XML文件中加载数据表结构描述列表
	}//namespace db
}//namespace hgl
#endif//HGL_DB_TABLE_DESC_INCLUDE
