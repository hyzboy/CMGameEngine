#include<hgl/filesystem/EnumFile.h>
#include<hgl/LogInfo.h>

#include<windows.h>
#include<sys/types.h>

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

            if(config->folder_name.IsEmpty()
             &&config->find_name.IsEmpty())RETURN_ERROR(-4);

            OSString full_findname;
            int count=0;

            if(config->folder_name.IsEmpty())
            {
                full_findname=config->find_name;
            }
            else
            {
                full_findname=MergeFilename(config->folder_name,config->find_name);
            }

            WIN32_FIND_DATAW FindFileData;
            HANDLE hFind;

            hFind = FindFirstFileW(full_findname, &FindFileData);
            if (hFind == INVALID_HANDLE_VALUE)
                return(-1);

            FileInfo fi;
            int sub_count;

            do
            {
                if(strcmp(FindFileData.cFileName,OS_TEXT("."))==0
                || strcmp(FindFileData.cFileName,OS_TEXT(".."))==0)
                {
                    continue;
                }

                if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
                {
                    if(!config->proc_folder)continue;
                }
                else
                {
                    if(!config->proc_file)continue;

                    ++count;
                }

                memset(&fi,0,sizeof(FileInfo));

                strcpy(fi.name,HGL_MAX_PATH,FindFileData.cFileName);

                if(config->folder_name.IsEmpty())
                {
                    strcpy(fi.fullname, HGL_MAX_PATH, fi.name);
                }
                else
                {
                    strcpy(fi.fullname, HGL_MAX_PATH, config->folder_name);

                    if(config->folder_name.GetEndChar()!=HGL_DIRECTORY_SEPARATOR)
                        strcat(fi.fullname, HGL_MAX_PATH, HGL_DIRECTORY_SEPARATOR);

                    const int rp =config->find_name.FindChar(HGL_DIRECTORY_SEPARATOR);//防止查询名称内仍有路径

                    if(rp!=-1)
                        strcat(fi.fullname, HGL_MAX_PATH,config->find_name.c_str(),rp);

                    strcat(fi.fullname, HGL_MAX_PATH, fi.name, HGL_MAX_PATH);
                }

                fi.size =   FindFileData.nFileSizeHigh;
                fi.size <<= 32;
                fi.size |=  FindFileData.nFileSizeLow;

                fi.is_hiddle=FindFileData.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN;

                fi.can_read =true;
                fi.can_write=!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_READONLY);

                fi.mtime=*(uint64 *)&FindFileData.ftLastWriteTime;

                if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
                {
                    fi.is_file=false;
                    fi.is_directory=true;

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
                    fi.is_file=true;
                    fi.is_directory=false;

                    ProcFile(config,fi);
                }
            }
            while(FindNextFileW(hFind, &FindFileData));

            FindClose(hFind);

            return(count);
        }
    }//namespace filesystem
}//namespace hgl
