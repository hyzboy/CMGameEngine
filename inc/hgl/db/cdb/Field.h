#ifndef HGL_CDB5_FIELD_INCLUDE
#define HGL_CDB5_FIELD_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
    namespace cdb
    {
		/**
		* 字段类型枚举
		*/
		enum FieldType
		{
			ftNone=0,

            //基本数据类型

			ftBool,

			ftInt8,			ftInt16,			ftInt32,			ftInt64,
			ftUint8,		ftUint16,			ftUint32,			ftUint64,

			ftFloat,        ftDouble,

            //角度
            ftAngleInt16,   ftAngleFloat,       ftAngleDouble,

            //弧度
            ftRadianFloat,  ftRadianDouble,

            ftCombo,            ///<以此为界，以下为组合方式

            ftDate,             ///<32位Year(2),Month(1),Day(1)
            ftTime,             ///<32位0至(24*60*60*1000)-1
            ftDateTime,

            ftVertex,           ///<坐标组合
            ftDirection,        ///<方向组合
            ftColor,            ///<颜色组合

            ftMatrix,           ///<矩阵组合

            ftArray,            ///<无序阵列
            ftSortedArray,      ///<有序阵列
            ftSet,              ///<合集(无重复数据无序阵列)
            ftSortedSet,        ///<有序合集
            ftKeyValue,         ///<KV阵列
            ftStack,            ///<堆栈(先进后出)
            ftQueue,            ///<队列(先进先出)
            ft2DBitmap,         ///<2D位图阵列
            ft3DBitmap,         ///<3D位图阵列
            ftTree,             ///<树

            ftTable,            ///<表格

            ftAny,              ///<任意组合

            ftEnd
        };//enum FieldType

        /**
        * 字段名称定义
        */
        constexpr char FieldBaseTypeName[][16]=
        {
            "bool",

            "int8",     "int16",    "int32",    "int64",
            "uint8",    "uint16",   "uint32",   "uint64",

            "float",
            "double",

            "angle_int16",
            "angle_float",
            "angle_double",

            "radian_float",
            "radian_double",

            "combo",

            "date",         "time",         "date_time",
            "vertex",       "direction",    "color",

            "matrix",

            "array",        "sorted_array",
            "set",          "sorted_set",

            "key_valye",

            "stack",        "queue",
            "2d_bitmap",    "3d_bitmap",

            "tree",

            "table",

            "any"
        };

        constexpr uint32 FieldBaseTypeBytes[]={1,8,16,32,64,8,16,32,64,32,64,16,32,64,32,64};   ///<基础字段单个数据所占位数

        /**
         * 字段定义
         */
        class FieldDefine               ///<字段定义
        {
        private:

            FieldType   type;           ///<基本类型

        public:

            FieldDefine(const FieldType &ft)
            {
                type=ft;
            }

            virtual ~FieldDefine()=default;

        public:

            const FieldType &GetBaseType()const{return type;}                   ///<取得字段基本类型

        public:

            virtual const char *GetName()const=0;                               ///<取得字段类型名称
            virtual const uint32 GetBytes(const uint32 &)const=0;               ///<返回当前字段多个数据所占字节数
        };//class FieldDefine
    }//namespace cdb
}//namespace hgl
#endif//HGL_CDB5_FIELD_INCLUDE
