#ifndef HGL_DB_RECORD_ROW_INCLUDE
#define HGL_DB_RECORD_ROW_INCLUDE

#include<hgl/type/List.h>
#include<hgl/db/Field.h>
#include<hgl/db/TableDesc.h>
namespace hgl
{
	namespace db
	{
		class FieldDesc;

		/**
		 * 行记录
		 */
		class RecordRow
		{
			const TableDesc *table_desc;

		protected:

			int field_data_count;																	///<数据字段数量
			FieldData **field_data_list;															///<字段数据列表

		protected:

			template<typename ...T> const bool add_field_data(const FieldDesc *desc,T...value)	///<设置一个字段的数据
			{
				FieldData *data=CreateFieldData(desc,value...);

				if(!data)return(false);

				field_data_list[desc->serial]=data;
				++field_data_count;
				return(true);
			}

		public:

			const	TableDesc *	GetTableDesc()const{return table_desc;}								///<取得表描述
					FieldData **GetFieldList()const{return field_data_list;}						///<取得字段数据列表
			const	int			GetFieldDataCount()const{return field_data_count;}					///<取得有数据的字段数量

		public:

			RecordRow(TableDesc *td):table_desc(td)
			{
				field_data_count=0;

				if(!table_desc)
				{
					field_data_list=nullptr;
					return;
				}

				const int field_count=table_desc->GetFieldCount();

				if(field_count<=0)
				{
					field_data_list=nullptr;
					return;
				}

				field_data_list=new FieldData *[field_count];
				memset(field_data_list,0,field_count*sizeof(FieldData *));
			}

			virtual ~RecordRow()
			{
				if(!field_data_list)
					return;

				const int field_count=table_desc->GetFieldCount();

				SAFE_CLEAR_OBJECT_ARRAY(field_data_list,field_count);
			}

			/**
			 * 根据序列号取得一个字段描述
			 */
			const FieldDesc *GetFieldDesc(int index)
			{
				if(!table_desc)return(nullptr);

				return table_desc->GetField(index);
			}

			/**
			* 根据字段名取得一个字段描述
			*/
			const FieldDesc *GetFieldDesc(const UTF8String &field_name)
			{
				if(!table_desc)return(nullptr);

				return table_desc->GetField(field_name);
			}

			/**
			* 根据序列号取得一个字段数据
			*/
			FieldData *GetField(int index)
			{
				if(!field_data_list)return(nullptr);

				if(index<0||index>=table_desc->GetFieldCount())
					return(nullptr);

				return field_data_list[index];
			}

			/**
			 * 根据字段名取得一个字段数据
			 */
			FieldData *GetField(const UTF8String &field_name)
			{
				if(!field_data_list)return(nullptr);

				const FieldDesc *fd=table_desc->GetField(field_name);

				if(!fd)return(nullptr);

				if(fd->serial<0||fd->serial>=table_desc->GetFieldCount())
					return(nullptr);

				return field_data_list[fd->serial];
			}

			template<typename I,typename ...T> const bool Set(const I &index,T...value)				///<设置一个字段的数据
			{
				const FieldDesc *desc=GetFieldDesc(index);

				if(!desc)return(false);

				if(field_data_list[desc->serial])
					return field_data_list[desc->serial]->Set(value...);

				return add_field_data(desc,value...);
			}

			template<typename I,typename ...T> const bool Get(const I &index,T...value)				///<取得一个字段的数据
			{
				FieldData *fd=GetField(index);

				if(!fd)return(false);

				return fd->Get(value...);
			}

			template<typename I,typename ...T> const bool Add(const I &index,T...value)				///<增加一个字段的数据
			{
				const FieldDesc *desc=GetFieldDesc(index);

				if(!desc)return(false);

				if(field_data_list[desc->serial])return(false);

				return add_field_data(desc,value...);
			}

			bool SetFieldData(const int index,FieldData *data)
			{
				if(index<0||index>=table_desc->GetFieldCount())return(false);
				if(!data)return(false);

				if(data->GetDesc()!=table_desc->GetField(index))return(false);

				if(field_data_list[index])return(false);

				field_data_list[index]=data;
				return(true);
			}
		};//class RecordRow
	}//namespace db
}//namespace hgl
#endif//HGL_DB_RECORD_ROW_INCLUDE
