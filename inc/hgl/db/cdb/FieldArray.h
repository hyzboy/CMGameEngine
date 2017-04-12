#ifndef HGL_CDB5_FIELD_ARRAY_INCLUDE
#define HGL_CDB5_FIELD_ARRAY_INCLUDE

#include<hgl/db/cdb/Field.h>
#include<hgl/LogInfo.h>
namespace hgl
{
    namespace cdb
    {
        class FieldArray:public FieldDefine
        {
            const FieldDefine *single_field_define;                             ///<单个数据的定义
            uint32 max_count;                                                   ///<阵列最大数据个数

        protected:

            friend FieldArray *CreateFieldArray(const FieldDefine *,const uint32 &);

            FieldArray(const FieldDefine *fd,const uint32 &mc):FieldDefine(ftArray)
            {
                single_field_define=fd;
                max_count=mc;
            }

        public:

            virtual ~FieldArray()=default;

            virtual const char *GetName()const override                         ///<取得字段类型名称
            {
                return FieldBaseTypeName[ftArray-ftBool];
            }

            const uint32 GetMaxCount()const                                     ///<取得阵列最大数据个数
            {
                return max_count;
            }

            virtual const uint32 GetBytes(const uint32 &num=1)const override    ///<取得字段数据长度
            {
                return single_field_define->GetBytes(num);
            }

            virtual const uint32 GetMaxBytes()const                             ///<取得字段最大数据长度
            {
                return single_field_define->GetBytes(max_count);
            }
        };//class FieldArray

        /**
         * 创建一个阵列字段
         * @param fd 单个字段定义
         * @param count 阵列最大数据个数
         */
        inline FieldArray *CreateFieldArray(const FieldDefine *fd,const uint32 &count)
        {
            if(!fd)RETURN_ERROR_NULL;
            if(count<=0)RETURN_ERROR_NULL;

            return(new FieldArray(fd,count));
        }
    }//namespace cdb
}//namespace hgl
#endif//HGL_CDB5_FIELD_ARRAY_INCLUDE
