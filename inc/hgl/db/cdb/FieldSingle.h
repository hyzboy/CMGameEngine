#ifndef HGL_CDB5_FIELD_SINGLE_INCLUDE
#define HGL_CDB5_FIELD_SINGLE_INCLUDE

#include<hgl/db/cdb/Field.h>
#include<hgl/LogInfo.h>
namespace hgl
{
    namespace cdb
    {
        /**
         * 独立字段(仅一个数据)
         */
        class FieldSingle:public FieldDefine
        {
            const char *single_name;
            uint32 single_bits;                                                 ///<单个数据所占位数

        private:

            friend FieldSingle *CreateFieldSingle(const FieldType &);

            FieldSingle::FieldSingle(const FieldType &ft):FieldDefine(ft)
            {
                int index=this->GetBaseType()-ftBool;

                single_name=FieldBaseTypeName[index];
                single_bits=FieldBaseTypeBytes[index];
            }

        public:

            const char *GetName()const override                                 ///<取得字段类型名称
            {
                return single_name;
            }

            const uint32 GetBytes(const uint32 &num=1)const override            ///<取得字段数据长度
            {
                return ((num*single_bits)+7)/8;
            }
        };//class FieldSingle

        /**
         * 独立一个独立字段定义对象
         */
        inline FieldSingle *CreateFieldSingle(const FieldType &ft)
        {
            if(ft<=ftNone||ft>=ftCombo)RETURN_ERROR_NULL;

            return(new FieldSingle(ft));
        }
    }//namespace cdb
}//namespace hgl
#endif//HGL_CDB5_FIELD_SINGLE_INCLUDE
