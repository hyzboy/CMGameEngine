#include<hgl/filesystem/FileSystem.h>
#include<hgl/LogInfo.h>
#include<hgl/io/FileInputStream.h>
#include<hgl/io/FileOutputStream.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#include <sys/types.h>
#include <string.h>
#include <errno.h>

namespace hgl
{
    namespace filesystem
    {
        constexpr int FILE_PROC_BUF_SIZE=HGL_SIZE_1MB;

        /**
        * 复制一个文件
        * @param sourcename 源文件名
        * @param targetname 目标文件名
        * @return 文件是否复制成功
        */
        bool FileCopy(const OSString &targetname,const OSString &sourcename)
        {
            io::FileInputStream fis;
            io::FileOutputStream fos;

            if(!fis.Open(sourcename))return(false);
            if(!fos.Create(targetname))return(false);

            size_t buf_size=FILE_PROC_BUF_SIZE;
            size_t length=fis.GetSize();

            size_t cur;

            if(buf_size>length)
                buf_size=length;

            SharedPtr<char> buf=new char[buf_size];

            while(length)
            {
                if(length>buf_size)
                    cur=buf_size;
                else
                    cur=length;

                if(fis.Read(buf,cur))
                    if(fos.Write(buf,cur))
                    {
                        length-=cur;
                        continue;
                    }

                return(false);
            }

            return(true);
        }

        /**
        * 删除一个文件
        * @param filename 文件名
        * @return 文件是否成功删除
        */
        bool FileDelete(const OSString &filename)
        {
            return(unlink(filename.c_str())==0);
        }

        /**
        * 移动一个文件
        * @param sourcename 源文件名
        * @param targetname 目标文件名
        * @return 文件是否移动成功
        */
        bool FileMove(const OSString &targetname,const OSString &sourcename)
        {
            if(FileCopy(sourcename,targetname))
                return FileDelete(sourcename);

            return(false);
        }

        /**
        * 修改文件名
        * @param oldname 旧的文件名
        * @param newname 新的文件名
        * @return 文件名是否修改成功
        */
        bool FileRename(const OSString &newname,const OSString &oldname)
        {
            return(rename(oldname.c_str(),
                        newname.c_str())==0);
        }

        /**
        * 确认文件是否存在
        * @param filename 要查找的文件名称
        * @return 这个文件是否存在
        */
        bool FileExist(const OSString &filename)
        {
            return access(filename.c_str(),F_OK)>=0;
        }

        /**
        * 检测文件是否可读
        * @param filename 文件名
        */
        bool FileCanRead(const OSString &filename)
        {
            return access(filename.c_str(),R_OK)>=0;
        }

        /**
        * 检测文件是否可写
        * @param filename 文件名
        */
        bool FileCanWrite(const OSString &filename)
        {
            return access(filename.c_str(),W_OK)>=0;
        }

        /**
        * 检测文件是否可执行
        * @param filename 文件名
        */
        bool FileCanExec(const OSString &filename)
        {
            return access(filename.c_str(),X_OK)>=0;
        }

        /**
        * 判断当前名称是否是一个目录
        * @param name 名称
        */
        bool IsDirectory(const os_char *name)
        {
            struct_stat64 buf;

            memset(&buf,0,sizeof(struct_stat64));

            if(hgl_lstat64(name,&buf)==-1)
                return(false);              //错误，有可能是不能访问

            return S_ISDIR(buf.st_mode);
        }

        /**
         * 判断当前名称是否是一个链接
         */
        bool IsLink(const os_char *name)
        {
            struct_stat64 buf;

            memset(&buf,0,sizeof(struct_stat64));

            if(hgl_lstat64(name,&buf)==-1)
                return(false);              //错误，有可能是不能访问

            return S_ISLNK(buf.st_mode);
        }

        bool MakeDirectory(const os_char *name)
        {
            if(!mkdir(name,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))return(true);

            LOG_PROBLEM(OS_TEXT("Create Directory <")+OSString(name)+OS_TEXT("> failed,errno: ")+OSString(errno));
            return(false);
        }

        os_char *GetRootPath(os_char *str)
        {
            if(str[0]==HGL_DIRECTORY_SEPARATOR)
                return str+1;
            else
                return str;
        }

        /**
        * 删除一个子目录
        * @param name 目录名称
        * @return 目录是否删除成功
        */
        bool DeletePath(const OSString &name)
        {
            return rmdir(name.c_str());
        }

        /**
        * 取得当前所在目录<br>
        */
        bool GetCurrentPath(OSString &path)
        {
            const int len=pathconf(".",_PC_PATH_MAX);

            if(len<=0)return(false);

            char *str=new char[len];

            memset(str,0,len);

            if(!getcwd(str,len))
            {
                delete[] str;
                return(false);
            }

            path.Set(str,strlen(str),true);
            return(true);
        }
    }//namespace filesystem
}//namespace hgl


