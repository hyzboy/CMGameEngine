#include"FileFilterCheck.h"
#include<sys/fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<iostream>

FileFilterCheck::~FileFilterCheck()
{
    if(in!=-1)close(in);
    if(out!=-1)close(out);
}

bool FileFilterCheck::Check(const char *origin_filename,const char *target_filename)
{
    in=open64(origin_filename,O_RDONLY);

    if(in<0)
    {
        std::cerr<<"openfile error("<<in<<") filename:"<<origin_filename<<std::endl;
        return(false);
    }

    out=open64(target_filename,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

    if(out<0)
    {
        std::cerr<<"create file error("<<out<<") filename:"<<target_filename<<std::endl;
        return(false);
    }

    struct stat file_stat;

    fstat(in,&file_stat);

    origin_size=file_stat.st_size;

    uint size;
    uint64 pos=0;

    MD5 md5check;

    md5check.Init();

    start_filter();

    while(pos<origin_size)
    {
        size=FILE_TMP_SIZE;

        if(pos+size>origin_size)
            size=origin_size-pos;

        size=read(in,buf,size);

        if(size<=0)
        {
            std::cerr<<"read file error:"<<size<<",filename:"<<origin_filename<<std::endl;
            return(false);
        }

        md5check.Update(buf,size);

        filter(buf,size);

        pos+=size;
    }

    close(out);
    out=-1;
    close(in);
    in=-1;

    md5check.Final(origin_md5);
    end_filter();
}

