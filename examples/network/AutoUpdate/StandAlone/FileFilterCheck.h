#pragma once
#include"md5.h"

constexpr uint FILE_TMP_SIZE=1024*1024;

/**
 * 文件数据过滤
 */
class FileFilterCheck
{
    int in;
    int out;

    char buf[FILE_TMP_SIZE];

private:

    virtual void start_filter()=0;
    virtual bool filter(void *data,const uint size)=0;                      ///<数据过滤处理函数
    virtual void end_filter()=0;

public:

    uint64 origin_size;         ///<原始长度
    MD5Code origin_md5;         ///<原始数据MD5

public:

    FileFilterCheck()
    {
        origin_size=0;
        in=-1;
        out=-1;
    }

    virtual ~FileFilterCheck();

    bool Check(const char *source_filename,const char *target_filename);
};//class FileFilterCheck

