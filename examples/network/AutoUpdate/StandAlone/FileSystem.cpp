#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>

namespace filesystem
{
    /**
    * 加载一个文件到内存,文件数据请自己delete[]掉
    * @param filename 要载入的文件名称
    * @param size 文件长度
    * @return 用来保存数据的内存指针的指针，请自行delete[]
    */
    char *LoadFileToMemory(const std::string &filename,unsigned int *size)
    {
        if(filename.empty())return(nullptr);
        if(!size)return(nullptr);

        int fp;
        int err;

        fp=open64(filename.c_str(),O_RDONLY);

        if(fp==-1)
        {
            std::cout<<"openfile error("<<err<<") filename:"<<filename.c_str()<<std::endl;
            return(nullptr);
        }

        struct stat file_stat;

        fstat(fp,&file_stat);

        *size=file_stat.st_size;

//            std::wcout<<L"OpenFile "<<*size<<L", "<<filename.c_str()<<std::endl;

        if(*size<=0)
            return(nullptr);

        char *ptr=new char[*size];

        if(read(fp,ptr,*size)!=*size)
        {
            delete[] ptr;
            ptr=nullptr;
        }

        close(fp);

        return ptr;
    }

    /**
    * 保存一块内存成文件
    * @param filename 要保存的文件名称
    * @param buf 要保存的内存指针
    * @param size 要保存的内存数据长度
    * @return 成功写入的字节数
    * @return -1 失败
    */
    unsigned int SaveMemoryToFile(const std::string &filename,const void *buf,unsigned int size)
    {
        if(filename.empty())return(-1);
        if(!buf)return(-2);
        if(size<=0)return(-3);

        int fp;
        int err;

        fp=open64(filename.c_str(),O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

        if(fp==-1)
        {
            std::cout<<"create file error("<<err<<") filename:"<<filename.c_str()<<std::endl;
            return(-4);
        }

        int result=write(fp,buf,size);
        close(fp);

        return result;
    }

    /**
    * 复制一个文件
    * @param sourcename 源文件名
    * @param targetname 目标文件名
    * @return 文件是否复制成功
    */
    bool FileCopy(const std::string &targetname,const std::string &sourcename)
    {
        int src;
        int dst;

        src=open64(sourcename.c_str(),O_RDONLY);

        if(src==-1)
            return(false);

        dst=open64(targetname.c_str(),O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

        if(dst==-1)
        {
            close(src);
            return(false);
        }

        size_t buf_size=1024*1024*8;
        size_t length;
        struct stat file_stat;

        fstat(src,&file_stat);

        length=file_stat.st_size;

        size_t cur;

        if(buf_size>length)
            buf_size=length;

        char *buf=new char[buf_size];

        bool result=false;

        while(length)
        {
            if(length>buf_size)
                cur=buf_size;
            else
                cur=length;

            if(read(src,buf,cur))
                if(write(dst,buf,cur))
                {
                    length-=cur;
                    continue;
                }

            break;
        }

        delete[] buf;
        close(src);
        close(dst);

        return(length==0);
    }

    /**
    * 确认文件是否存在
    * @param filename 要查找的文件名称
    * @return 这个文件是否存在
    */
    bool FileConfirm(const std::string &filename)
    {
        return access(filename.c_str(),F_OK)>=0;
    }

    /**
    * 删除一个文件
    * @param filename 文件名
    * @return 文件是否成功删除
    */
    bool FileDelete(const std::string &filename)
    {
        return(unlink(filename.c_str())==0);
    }

    /**
    * 检测文件是否可执行
    * @param filename 文件名
    */
    bool FileCanExec(const std::string &filename)
    {
        return access(filename.c_str(),X_OK)>=0;
    }

    /**
    * 判断当前名称是否是一个目录
    * @param name 名称
    */
    bool IsDirectory(const std::string &name)
    {
        struct stat64 buf;

        memset(&buf,0,sizeof(struct stat64));

        if(lstat64(name.c_str(),&buf)==-1)
            return(false);				//错误，有可能是不能访问

        return S_ISDIR(buf.st_mode);
    }

    bool MakeDirectory(const std::string &name)
    {
        if(!mkdir(name.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))return(true);

        return(false);
    }

    char *GetRootPath(char *str)
    {
        if(str[0]=='/')
            return str+1;
        else
            return str;
    }

    /**
    * 创建一个子目录,该函数可自动创建多级目录。
    * @param dirname 目录名称
    * @return 目录是否创建成功
    */
    bool MakePath(const std::string &dirname)
    {
        const char directory_separator='/';
        char *p;

        char str[_POSIX_PATH_MAX];
        char *sp;

        strcpy(str,dirname.c_str());

        sp=GetRootPath(str);

        while(1)
        {
            p=strchr(sp,directory_separator);

            if(p)
                *p=0;

            if(*sp==0)
                return(true);

            if(!IsDirectory(str))//没有找到
                if(!MakeDirectory(str))
                    return(false);

            if(p)
                *p++=directory_separator;
            else
                return(true);

            sp=p;
        }
    }
}
