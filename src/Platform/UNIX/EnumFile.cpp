#include<hgl/filesystem/EnumFile.h>
#include<hgl/LogInfo.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>

namespace hgl
{
    namespace filesystem
    {
        /**
        * 枚举一个目录内的所有文件
        * @param config 枚举配置
        * @return 查找到文件数据,<0表示失败
        */
        int EnumFile::Enum(EnumFileConfig *config)
        {
            if(!config)RETURN_ERROR(-1);

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
            if((entry = hgl_readdir64(dir)) == NULL)
                return(-1);

            int sub_count;

            do
            {
                if(strcmp(entry->d_name,".")==0
                 ||strcmp(entry->d_name,"..")==0)
                {
                    continue;
                }

                memset(&statbuf,0,sizeof(struct_stat64));

                if(hgl_lstat64(entry->d_name,&statbuf)==-1)
                    continue;

                if(S_ISDIR(statbuf.st_mode))
                {
                    if(!config->proc_folder)continue;
                }
                else
                {
                    if(!config->proc_file)continue;

                    ++count;
                }

                memset(&fi,0,sizeof(FileInfo));
                fi.size=statbuf.st_size;

                fi.can_read =statbuf.st_mode&S_IROTH;
                fi.can_write=statbuf.st_mode&S_IWOTH;

                fi.is_hiddle=(entry->d_name[0]=='.');

                fi.mtime=statbuf.st_mtime;

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
                    EnumFileConfig *sub_efc=CreateSubConfig(config,fi);

                    if(sub_efc&&config->sub_folder)
                    {
                        sub_count=this->Enum(sub_efc);
                        if(sub_count>0)count+=sub_count;
                    }

                    ProcFolder(config,sub_efc,fi);

                    if(sub_efc)
                        delete sub_efc;
                }
                else
                {
                    ProcFile(config,fi);
                }
            }
            while((entry=hgl_readdir64(dir)));

            closedir(dir);

            chdir(path_buf);
            return(count);
        }
    }//namespace filesystem
}//namespace hgl
