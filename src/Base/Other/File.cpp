#include <hgl/FileSystem.h>
#include <hgl/LogInfo.h>
#include <hgl/io/FileInputStream.h>
#include <hgl/io/FileOutputStream.h>

#include <sys/types.h>
#include <string.h>

namespace hgl
{
    namespace filesystem
    {
        constexpr int FILE_PROC_BUF_SIZE=HGL_SIZE_1MB;

        /**
         * 组合路径名与文件名
         * @param fullname 完整路径文件名
         * @param pathname 路径名
         * @param filename 文件名
         */
        void MergeFilename(OSString &fullname,const OSString &pathname,const OSString &filename)
        {
            fullname=(  pathname.GetEndChar()==HGL_DIRECTORY_SEPARATOR?
                        pathname+filename:
                        pathname+HGL_DIRECTORY_SEPARATOR+filename);
        }

        /**
        * 比较两个文件是否一样
        * @param filename1 第一个文件的名称
        * @param filename2 第二个文件的名称
        * @return 文件是否一样
        */
        bool FileComp(const OSString &filename1,const OSString &filename2)
        {
            io::FileInputStream fp1,fp2;
            int64 fs1,fs2;

            if(!fp1.Open(filename1))return(false);
            if(!fp2.Open(filename2))return(false);

            fs1=fp1.GetSize();
            fs2=fp2.GetSize();

            if(fs1!=fs2)
                return(false);

            int64 pos=0,size;
            char *data1,*data2;

            data1=new char[FILE_PROC_BUF_SIZE];
            data2=new char[FILE_PROC_BUF_SIZE];

            while(pos<fs1)
            {
                size=FILE_PROC_BUF_SIZE;
                if(pos+size>fs1)size=fs1-pos;

                fp1.Read(data1,size);
                fp2.Read(data2,size);

                if(memcmp(data1,data2,size)==0)
                {
                    pos+=size;
                    continue;
                }
                else
                {
                    delete[] data1;
                    delete[] data2;
                    return(false);
                }
            };

            delete[] data1;
            delete[] data2;
            return(true);
        }

        /**
        * 加载一个文件到内存,文件数据请自己delete[]掉
        * @param filename 要载入的文件名称
        * @param buf 用来保存数据的内存指针的指针
        * @return 文件长度
        */
        int64 LoadFileToMemory(const OSString &filename,void **buf)
        {
            io::FileInputStream fs;

            if(!fs.Open(filename))
                return(-1);

            const int64 size=fs.GetSize();

            char *fb=new char[size];

            if(fs.Read(fb,size)==size)
            {
                *buf=fb;
                return(size);
            }

            delete[] fb;
            return(-1);
        }

        /**
        * 保存一块内存成文件
        * @param filename 要保存的文件名称
        * @param buf 要保存的内存指针
        * @param size 要保存的内存数据长度
        * @return 成功写入的字节数
        * @return -1 失败
        */
        int64 SaveMemoryToFile(const OSString &filename,const void *buf,int64 size)
        {
            io::FileOutputStream fs;

            if(!fs.CreateTrunc(filename))
                return(-1);

            return fs.Write(buf,size);
        }

        os_char *GetRootPath(os_char *str);
        bool MakeDirectory(const OSString &name);

        /**
        * 创建一个子目录,该函数可自动创建多级目录。
        * @param dirname 目录名称
        * @return 目录是否创建成功
        */
        bool MakePath(const OSString &dirname)
        {
            const os_char directory_separator=HGL_DIRECTORY_SEPARATOR;
            os_char *p;

            os_char str[HGL_MAX_PATH];
            os_char *sp;

            strcpy(str,HGL_MAX_PATH,dirname.c_str());

            sp=GetRootPath(str);

            while(1)
            {
                p=hgl::strchr(sp,directory_separator);

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

        /**
        * 删除一个子目录,包含所有文件和子目录
        * @param dirname 目录名称
        * @return 目录是否删除成功
        */
    //     void DeleteTree(const OSString &dirname)
    //     {
    //     }

        /**
        * 读取文件的一部分到内存
        * @param filename 文件名
        * @param offset 起始地址
        * @param length 读取长度
        * @param buf 数据存放地址,为NULL表示要求函数分配
        * @return NULL 读取失败
        * @return 读取出来的数据存放的地址
        */
        void *LoadFileToMemory(const OSString &filename,int64 offset,void *buf,int64 length)
        {
            if(!filename||!*filename||length==0)
                return(nullptr);

            io::FileInputStream fs;

            if(!fs.Open(filename))
                return(nullptr);

            const int64 file_length=fs.GetSize();

            if(offset+length>file_length)
            {
                LOG_PROBLEM(OS_TEXT("读取文件<")+filename+OS_TEXT("><")+OSString(offset)+OS_TEXT(",")+OSString(length)+OS_TEXT(">超出了范围，文件长度为<")+OSString(file_length));
                return(nullptr);
            }

            char *fb;

            if(buf)
                fb=(char *)buf;
            else
                fb=new char[length];

            if(fs.Read(offset,fb,length)==length)
            {
                LOG_INFO(OS_TEXT("加载文件<")+filename+OS_TEXT("><")+OSString(offset)+OS_TEXT(",")+OSString(length)+OS_TEXT(">到缓冲区成功."));

                return(buf);
            }
            else
            {
                if(fb!=buf)
                    delete[] fb;

                return(nullptr);
            }
        }

        /**
        * 保存内存中的数据到文件中
        * @param filename 文件名
        * @param offset 起始地址
        * @param length 数据长度
        * @param data 数据长度
        * @return 是否成功
        */
        bool SaveMemoryToFile(const OSString &filename,int64 offset,const void *data,int64 length)
        {
            io::FileOutputStream fs;

            if(!fs.Open(filename))
                return(false);

            return fs.Write(offset,data,length);
        }

        bool GetFileInfo(const OSString &filename,struct FileInfo &fi)
        {
            struct_stat64 file_state;

            memset(&file_state,0,sizeof(struct_stat64));

            if(lstat64(filename.c_str(),&file_state)==-1)
                return(false);

            memset(&fi,0,sizeof(FileInfo));

            if(file_state.st_mode&S_IFREG)
            {
                fi.is_file=true;
                fi.is_directory=false;
            }

            if(file_state.st_mode&S_IFDIR)
            {
                fi.is_file=false;
                fi.is_directory=true;
            }

            fi.size=file_state.st_size;
            return(true);
        }

        namespace
        {
            struct EnumFileInfo :public EnumFileConfig
            {
                List<FileInfo> *fi_list;
            };
        }
        
        EnumFileConfig *DefaultCreateSubConfig(struct EnumFileConfig *efc,const OSString &sub_folder_name)
        {
            OSString full_sub_folder_name;

            MergeFilename(full_sub_folder_name,efc->folder_name,sub_folder_name);

            return(new EnumFileConfig(efc,full_sub_folder_name));
        }

        void AddToList(struct EnumFileConfig *efc,hgl::filesystem::FileInfo &fi)
        {
            EnumFileInfo *efi = (EnumFileInfo *)efc;

            efi->fi_list->Add(fi);
        }

        int GetFileInfoList(List<FileInfo> &fi_list,const OSString &folder_name,bool proc_folder,bool proc_file,bool sub_folder)
        {
            EnumFileInfo efc;

            efc.fi_list     = &fi_list;

            efc.folder_name = folder_name;
            efc.proc_folder = proc_folder;
            efc.proc_file   = proc_file;
            efc.sub_folder  = sub_folder;

            efc.cb_file = AddToList;

            return EnumFile(&efc);
        }
    }//namespace filesystem
}//namespace hgl
