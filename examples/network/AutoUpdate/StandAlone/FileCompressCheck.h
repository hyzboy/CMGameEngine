#pragma once
#include"FileFilterCheck.h"

class FileCompressCheck:public FileFilterCheck
{
    MD5 compress_md5check;

public:

    uint64 compress_size;       ///<压缩后长度

    MD5Code compress_md5;       ///<压缩数据MD5

public:

    FileCompressCheck()
    {
        compress_size=0;
    }

    ~FileCompressCheck() override =default;

    void start_filter()override;
    bool filter(void *data,const uint size)override;                      ///<数据过滤处理函数
    void end_filter()override;
};//class FileCompressCheck
