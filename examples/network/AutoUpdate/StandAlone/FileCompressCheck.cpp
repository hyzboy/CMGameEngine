#include"FileCompressCheck.h"
#include<sys/fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<iostream>

void FileCompressCheck::start_filter()
{
    compress_md5check.Init();
}

bool FileCompressCheck::filter(void *data,const uint size)
{
}

void FileCompressCheck::end_filter()
{
    compress_md5check.Final(compress_md5);
}

