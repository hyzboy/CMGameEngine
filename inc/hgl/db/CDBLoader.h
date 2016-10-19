#ifndef HGL_CDB_LOADER_INCLUDE
#define HGL_CDB_LOADER_INCLUDE

#include<hgl/db/CDBTable.h>
#include<hgl/db/CDBField.h>
#include<hgl/io/DataInputStream.h>
namespace hgl
{
	using namespace io;

	namespace db
	{
		class CDBFieldMap
		{
		public:

			CDBField *field;					///<对应字段
			size_t offset;						///<偏移

		public:

			CDBFieldMap(CDBField *f,size_t o)
			{
				field=f;
				offset=o;
			}

			virtual ~CDBFieldMap()=default;

			virtual void Convert(int,void *)=0;

			bool ToString(int index,UTF16String &str)
			{
				if(!field)return(false);

				return field->ToString(index,str);
			}
		};//class CDBFieldMap

		class CDBFieldMapBool:public CDBFieldMap
		{
		public:

			using CDBFieldMap::CDBFieldMap;

			void Convert(int row,void *base)
			{
				field->GetBool(row,*(bool *)(((char *)base)+offset));
			}
		};//class CDBFieldMapBool

		class CDBFieldMapInteger:public CDBFieldMap
		{
		public:

			using CDBFieldMap::CDBFieldMap;

			void Convert(int row,void *base)
			{
				field->GetInteger(row,*(int *)(((char *)base)+offset));
			}
		};//class CDBFieldMapInteger

		class CDBFieldMapUInteger:public CDBFieldMap
		{
		public:

			using CDBFieldMap::CDBFieldMap;

			void Convert(int row,void *base)
			{
				field->GetUInteger(row,*(uint *)(((char *)base)+offset));
			}
		};//class CDBFieldMapUInteger

		class CDBFieldMapFloat:public CDBFieldMap
		{
		public:

			using CDBFieldMap::CDBFieldMap;

			void Convert(int row,void *base)
			{
				field->GetFloat(row,*(float *)(((char *)base)+offset));
			}
		};//class CDBFieldMapFloat

		class CDBFieldMapDouble:public CDBFieldMap
		{
		public:

			using CDBFieldMap::CDBFieldMap;

			void Convert(int row,void *base)
			{
				field->GetDouble(row,*(double *)(((char *)base)+offset));
			}
		};//class CDBFieldMapDouble

		class CDBFieldMapChar8:public CDBFieldMap
		{
		public:

			using CDBFieldMap::CDBFieldMap;

			void Convert(int row,void *base)
			{
				field->GetChar(row,*(((char *)base)+offset));
			}
		};//class CDBFieldMapChar8

		class CDBFieldMapChar16:public CDBFieldMap
		{
		public:

			using CDBFieldMap::CDBFieldMap;

			void Convert(int row,void *base)
			{
				field->GetChar(row,*(u16char *)(((char *)base)+offset));
			}
		};//class CDBFieldMapChar16

		template<typename T> class CDBFieldMapArray:public CDBFieldMap
		{
		public:

			using CDBFieldMap::CDBFieldMap;

			void Convert(int row,void *base)
			{
				field->GetArray(row,(T *)(((char *)base)+offset));
			}
		};//class CDBFieldMapArray

		/**
		 * CDB表格数据加载转换置自定义结构实现类
		 */
		template<typename R> class CDBTableLoader
		{
			OSString filename;

			CDBTable table;

			ObjectList<CDBFieldMap> map_list;

		public:

			bool LoadFile(DataInputStream *dis)
			{
				return table.Load(dis);
			}

			bool LoadFile(const OSString &fn)
			{
				filename=fn;

#ifdef LOG_CDB_LOADER_LOG
				LOG_INFO(OS_TEXT("CDBTableLoader<>::LoadFile(")+fn+OS_TEXT(")"));
#endif//LOG_CDB_LOADER_LOG
				return table.Load(filename);
			}

			bool Map(const u16char *name,size_t offset)
			{
				db::CDBField *field=table.Fields[name];

				if(!field)
				{
					LOG_ERROR(u"CDBTableLoader::Map("+UTF16String(name)+u","+UTF16String((uint)offset)+u") don't find field.");
					return(false);
				}

				if(field->Type.count>1)
				{
					if(field->Type.base==fbtBool	){map_list.Add(new CDBFieldMapArray<bool	>(field,offset));return(true);}
					if(field->Type.base==fbtInt8	){map_list.Add(new CDBFieldMapArray<int8 	>(field,offset));return(true);}
					if(field->Type.base==fbtInt16	){map_list.Add(new CDBFieldMapArray<int16	>(field,offset));return(true);}
					if(field->Type.base==fbtInt32	){map_list.Add(new CDBFieldMapArray<int32	>(field,offset));return(true);}
// 					if(field->Type.base==fbtInt64	){map_list.Add(new CDBFieldMapArray<int64	>(field,offset));return(true);}
					if(field->Type.base==fbtUint8	){map_list.Add(new CDBFieldMapArray<uint8 	>(field,offset));return(true);}
					if(field->Type.base==fbtUint16	){map_list.Add(new CDBFieldMapArray<uint16	>(field,offset));return(true);}
					if(field->Type.base==fbtUint32	){map_list.Add(new CDBFieldMapArray<uint32	>(field,offset));return(true);}
// 					if(field->Type.base==fbtUint64	){map_list.Add(new CDBFieldMapArray<uint64	>(field,offset));return(true);}
					if(field->Type.base==fbtFloat	){map_list.Add(new CDBFieldMapArray<float	>(field,offset));return(true);}
					if(field->Type.base==fbtDouble	){map_list.Add(new CDBFieldMapArray<double	>(field,offset));return(true);}
					if(field->Type.base==fbtChar8	){map_list.Add(new CDBFieldMapArray<char	>(field,offset));return(true);}
					if(field->Type.base==fbtChar16le){map_list.Add(new CDBFieldMapArray<u16char>(field,offset));return(true);}
				}
				else
				{
					if(field->Type.base==fbtBool	){map_list.Add(new CDBFieldMapBool		(field,offset));return(true);}
					if(field->Type.base==fbtInt8
					 ||field->Type.base==fbtInt16
					 ||field->Type.base==fbtInt32	){map_list.Add(new CDBFieldMapInteger	(field,offset));return(true);}
					if(field->Type.base==fbtUint8
					 ||field->Type.base==fbtUint16
					 ||field->Type.base==fbtUint32	){map_list.Add(new CDBFieldMapUInteger	(field,offset));return(true);}
					if(field->Type.base==fbtFloat	){map_list.Add(new CDBFieldMapFloat		(field,offset));return(true);}
					if(field->Type.base==fbtDouble	){map_list.Add(new CDBFieldMapDouble	(field,offset));return(true);}
					if(field->Type.base==fbtChar8	){map_list.Add(new CDBFieldMapChar8		(field,offset));return(true);}
					if(field->Type.base==fbtChar16le){map_list.Add(new CDBFieldMapChar16	(field,offset));return(true);}
				}

				LOG_ERROR(u"CDBTableLoader::Map("+UTF16String(name)+u","+UTF16String((uint)offset)+u") don't parse field type.");
				return(false);
			}

			template<typename T>
			bool MapArray(const u16char *name,size_t offset)
			{
				db::CDBField *field=table.Fields[name];

				if(!field)return(false);

				map_list.Add(new CDBFieldMapArray<T>(field,offset));
				return(true);
			}

			virtual void MapType()=0;								///<映射数据类型
			virtual R *CreateRecord(){return(new R());}				///<创建一条记录
			virtual void Record(R *obj)=0;							///<处理一条记录

			void Parse()
			{
				const int row_count=table.GetDataCount();
				const int col_count=map_list.GetCount();

#ifdef LOG_CDB_LOADER_LOG
				LOG_INFO(OS_TEXT("CDBTableLoader::Parse(")+filename+OS_TEXT(") Start"));
#endif//LOG_CDB_LOADER_LOG

				for(int row=0;row<row_count;row++)
				{
					R *obj=CreateRecord();
					CDBFieldMap **m=map_list.GetData();

#ifdef LOG_CDB_LOADER_LOG
					UTF16String str_all=u"Record["+UTF16String(row)+u"]:";
					UTF16String str;
#endif//LOG_CDB_LOADER_LOG

					for(int col=0;col<col_count;col++)
					{
						(*m)->Convert(row,obj);

#ifdef LOG_CDB_LOADER_LOG
						if((*m)->ToString(row,str))
							str_all+=u' '+(*m)->field->Name+u":"+str;
#endif//LOG_CDB_LOADER_LOG

						m++;
					}

#ifdef LOG_CDB_LOADER_LOG
					LOG_INFO(str_all);
#endif//LOG_CDB_LOADER_LOG

					Record(obj);
				}

#ifdef LOG_CDB_LOADER_LOG
				LOG_INFO(OS_TEXT("CDBTableLoader::Parse(")+filename+OS_TEXT(") End"));
#endif//LOG_CDB_LOADER_LOG
			}//void Parse()

			bool Load(InputStream *is)
			{
				if(!is)return(false);

				SharedPtr<DataInputStream> dis=new LEDataInputStream(is);

				if(!LoadFile(dis))
					return(false);

				MapType();
				Parse();
				return(true);
			}

			bool Load(const OSString &filename)
			{
				if(!LoadFile(filename))
					return(false);

				MapType();
				Parse();
				return(true);
			}//bool Load(const OSString &filename)
		};//class CDBTableLoader
	}//namespace db
}//namespace hgl
#endif//HGL_CDB_LOADER_INCLUDE
