#ifndef HAC4_INCLUDE
#define HAC4_INCLUDE

#include"HacTemplate.h"
#include<hgl/Compress.h>
#include<hgl/ut/Hash.h>
namespace hgl
{
    namespace io
    {
        class InputStream;
        class DataInputStream;
    }//namespace io

    struct Hac4File
    {
        uint64      ID;             ///<文件ID

        UTF16String FileName;       ///<文件名
        UTF16String CompressName;   ///<压缩算法
        uint64      FileSize;       ///<文件长度
        uint64      CompressSize;   ///<压缩后大小
        MD5Code     md5;            ///<文件MD5校验码
        uint64      Offset;         ///<数据在包中的偏移量
    };//struct Hac4File

    class HAC4:public HacTemplate<Hac4File>
    {
        uint64 TotalSize;

        io::DataInputStream *stream;

        void LoadFolder(io::DataInputStream *,HacFolder<Hac4File> *);

    public:

        HAC4(io::DataInputStream *);
        ~HAC4();

        bool LoadFilePart(void *,uint,uint,void *);                                                 ///<加载一个文件的一部分

        io::InputStream *LoadFileFrom(void *,const UTF16String &,bool=false);                       ///<加载一个文件到内存流
        bool LoadFileFrom(void *,const UTF16String &,void **,int *);                                ///<加载一个文件到指定内存块
    };//class HAC4
}//namespace hgl
#endif//HAC4_INCLUDE
