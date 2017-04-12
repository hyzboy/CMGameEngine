#include<hgl/ut/XMLParse.h>
#include<hgl/db/TableDesc.h>
#include<hgl/type/Map.h>

namespace hgl
{
    namespace db
    {
        /**
        * 表格字段配置加载
        */
        class TableDescLoader:public XMLParse
        {
            TableDescList *table_desc_list;

            TableDesc *table;

        public:

            TableDescLoader(TableDescList *tdl)
            {
                table_desc_list=tdl;
                table=nullptr;
            }

            void LoadTable(const char **atts)
            {
                UTF8String name;

                XML_START_PARSE(atts)
                    xml_parse_string_u8(name)
                XML_END_PARSE();

                table=new TableDesc(name);

                LOG_INFO("Load Table: "+name);
            }

            void LoadField(const char **atts)
            {
                if(!table)return;

                UTF8String name,type;
                UTF8String key;
                UTF8String def_value;
                bool not_null=false;
                bool auto_inc=false;

                XML_START_PARSE(atts)
                    xml_parse_string_u8(name)
                    xml_parse_string_u8(type)
                    xml_parse_string_u8(key)
                    xml_parse_bool(not_null)
                    xml_parse_bool(auto_inc)
                    xml_parse_string_u8(def_value)
                XML_END_PARSE();

                FieldType ft=ParseFieldType(type);

                if(ft.base<=fbtNone||ft.base>=fbtEnd)
                {
                    LOG_INFO("Field type parse error name=\""+name+"\" type="+type);
                    return;
                }

                FieldDesc *fd;
                                                                        //主键,    索引,    唯一
                if(key.CaseComp("primary")==0)    fd=new FieldDesc(name,ft,true,    true,    true,    not_null,def_value,auto_inc);else
                if(key.CaseComp("unique")==0)    fd=new FieldDesc(name,ft,false,    true,    true,    not_null,def_value,auto_inc);else
                if(key.CaseComp("index")==0)    fd=new FieldDesc(name,ft,false,    true,    false,    not_null,def_value,auto_inc);else
                                                fd=new FieldDesc(name,ft,false,    false,    false,    not_null,def_value,auto_inc);

                if(!table->AddField(fd))
                    delete fd;

                LOG_INFO("Field name: \""+name+"\" type: "+type);
            }

            void LoadExtraAttrib(const char **atts)
            {
                XML_START_PARSE(atts)
                    if(hgl::strlen(flag)>0
                     &&hgl::strlen(info)>0)
                        table->AddExtraAttrib(flag,info);
                    else
                XML_END_PARSE();
            }

            void StartElement(const char *element_name,const char **atts)
            {
                if(strcmp(element_name,"table")==0)
                    LoadTable(atts);
                else
                if(strcmp(element_name,"field")==0)
                    LoadField(atts);
                else
                if(strcmp(element_name,"extra_attrib")==0)
                {
                    LoadExtraAttrib(atts);
                }
            }

            void EndElement(const char *element_name)
            {
                if(strcmp(element_name,"table")==0)
                {
                    if(table->GetFieldCount()<=1
                    ||table->GetPrimaryField()==nullptr)
                    {
                        delete table;
                    }
                    else
                    {
                        table_desc_list->Add(table->GetTableName(),table);
                    }

                    table=nullptr;
                }
            }

            bool Load(const char *data,int size)
            {
                Start();

                return Parse(data,size,true);
            }

            bool Load(const OSString &filename)
            {
                Start();

                return XMLParseFile(this,filename);
            }
        };//class TableDescLoader

        bool LoadTableDesc(TableDescList &table_desc_list,const char *data,int size)
        {
            TableDescLoader tdl(&table_desc_list);

            return tdl.Load(data,size);
        }

        bool LoadTableDesc(TableDescList &table_desc_list,const OSString &filename)
        {
            TableDescLoader tdl(&table_desc_list);

            return tdl.Load(filename);
        }
    }//namespace db
}//namespace hgl
