#include <hgl/FileSystem.h>
#include <hgl/LogInfo.h>
#include <hgl/io/FileInputStream.h>
#include <hgl/io/FileOutputStream.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#include <sys/types.h>
#include <string.h>

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
        bool FileConfirm(const OSString &filename)
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

            if(lstat64(name,&buf)==-1)
                return(false);				//错误，有可能是不能访问

            return S_ISDIR(buf.st_mode);
        }

        bool MakeDirectory(const OSString &name)
        {
            if(!mkdir(name,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))return(true);

            LOG_PROBLEM(OS_TEXT("创建目录<")+name+OS_TEXT(">失败！"));
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

        /**
        * 取得当前程序完整路径名称
        */
        bool GetCurrentProgram(OSString &result)
        {
            os_char *path=new os_char[HGL_MAX_PATH];

            const int len=readlink("/proc/self/exe",path,HGL_MAX_PATH);

            if(len<=0)
            {
                delete[] path;
                return(false);
            }

            path[len]=0;
            result.Set(path,len,true);

            return(true);
        }

        /**
        * 枚举一个目录内的所有文件
        * @param config 枚举配置
        * @return 查找到文件数据,<0表示失败
        */
        int EnumFile(EnumFileConfig *config)
        {
            if(!config)RETURN_ERROR(-1);

//            if(config->proc_folder&&!config->cb_folder)RETURN_ERROR(-2);            //这一行是不需要的，确实存在proc_folder=true,但没有cb_folder的情况。但留在这里。以防删掉后，未来没注意自以为是的加上这样一行
            if(config->proc_file&&!config->cb_file)RETURN_ERROR(-3);

            if(config->folder_name.IsEmpty())RETURN_ERROR(-4);

            OSString fullname;
            int count=0;

            if(config->folder_name.IsEmpty())
            {
                fullname='.';
            }
            else
            {
                fullname=config->folder_name;
            }

            DIR *dir;
            struct_dirent64 *entry;
            struct_stat64 statbuf;
            FileInfo fi;
            char path_buf[HGL_MAX_PATH]={0};
            getcwd(path_buf,HGL_MAX_PATH);

            chdir(fullname);
            if((dir = opendir(fullname)) == NULL)
                return(-1);
            if((entry = readdir64(dir)) == NULL)
                return(-1);

            EnumFileConfig *sub_efc=nullptr;
            int sub_count;

            do
            {
                if(strcmp(entry->d_name,".")==0
                ||strcmp(entry->d_name,"..")==0)
                {
                    continue;
                }

                memset(&statbuf,0,sizeof(struct_stat64));

                if(lstat64(entry->d_name,&statbuf)==-1)
                    continue;

                memset(&fi,0,sizeof(FileInfo));
                fi.size=statbuf.st_size;

                if(S_ISDIR(statbuf.st_mode))
                {
                    fi.is_file=false;
                    fi.is_directory=true;
                }
                else
                {
                    fi.is_file=true;
                    fi.is_directory=false;
                }

                fi.can_read	=statbuf.st_mode&S_IROTH;
                fi.can_write=statbuf.st_mode&S_IWOTH;

                if(S_ISDIR(statbuf.st_mode))
                {
                    if(!config->proc_folder)continue;

                    if(config->sub_folder)
                    {
                        sub_efc=config->CreateSubConfig(config,entry->d_name);

                        if(!sub_efc)
                            continue;

                        sub_count=EnumFile(sub_efc);
                        if(sub_count>0)count+=sub_count;
                    }
                }
                else
                {
                    if(!config->proc_file)continue;

                    ++count;
                }

                memset(&fi,0,sizeof(FileInfo));

                strcpy(fi.name,HGL_MAX_PATH,entry->d_name);

                if(config->folder_name.IsEmpty())
                {
                    strcpy(fi.fullname,HGL_MAX_PATH,fi.name);
                }
                else
                {
                    strcpy(fi.fullname,HGL_MAX_PATH,config->folder_name);

                    if(config->folder_name.GetEndChar()!=HGL_DIRECTORY_SEPARATOR)
                        strcat(fi.fullname,HGL_MAX_PATH,HGL_DIRECTORY_SEPARATOR);

                    strcat(fi.fullname,HGL_MAX_PATH,fi.name,HGL_MAX_PATH);
                }

                if(fi.is_directory)
                {
                    if(config->cb_folder)
                        config->cb_folder(config,sub_efc,fi);

                    delete sub_efc;
                    sub_efc=nullptr;
                }
                else
                {
                    if(config->cb_file)
                        config->cb_file(config,fi);
                }
            }
            while((entry=readdir64(dir)));

            closedir(dir);

            chdir(path_buf);
            return(count);
        }
    }//namespace filesystem
}//namespace hgl

