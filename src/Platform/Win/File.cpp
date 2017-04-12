#include <hgl/FileSystem.h>
#include <hgl/LogInfo.h>
#include <hgl/io/FileInputStream.h>
#include <hgl/io/FileOutputStream.h>

#include <windows.h>

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
        bool FileCopy(const OSString &sourcename,const OSString &targetname)
        {
            return(::CopyFileW(sourcename,targetname,false));
        }

        /**
        * 删除一个文件
        * @param filename 文件名
        * @return 文件是否成功删除
        */
        bool FileDelete(const OSString &filename)
        {
            return(::DeleteFileW(filename));
        }

        /**
        * 移动一个文件
        * @param sourcename 源文件名
        * @param targetname 目标文件名
        * @return 文件是否移动成功
        */
        bool FileMove(const OSString &sourcename,const OSString &targetname)
        {
            return(::MoveFileW(sourcename,targetname));
        }

        /**
        * 修改文件名
        * @param oldname 旧的文件名
        * @param newname 新的文件名
        * @return 文件名是否修改成功
        */
        bool FileRename(const OSString &oldname,const OSString &newname)
        {
            return(::MoveFileW(oldname,newname));
        }

        /**
        * 确认文件是否存在
        * @param filename 要查找的文件名称
        * @return 这个文件是否存在
        */
        bool FileConfirm(const OSString &filename)
        {
            WIN32_FIND_DATAW wfd;
            HANDLE hFind;

            hFind=FindFirstFileW(filename, &wfd);

            if(hFind==INVALID_HANDLE_VALUE)
                return(false);

            FindClose(hFind);
            return(true);
        }

        /**
        * 检测文件是否可读
        * @param filename 文件名
        */
        bool FileCanRead(const OSString &filename)
        {
            const DWORD attr=GetFileAttributesW(filename);

            if(attr==INVALID_FILE_ATTRIBUTES)
                return(false);

            if(attr&(FILE_ATTRIBUTE_DEVICE|FILE_ATTRIBUTE_DIRECTORY))    //不是个文件
                return(false);

            if(attr&FILE_ATTRIBUTE_OFFLINE)
                return(false);

            return(true);
        }

        /**
        * 检测文件是否可写
        * @param filename 文件名
        */
        bool FileCanWrite(const OSString &filename)
        {
            const DWORD attr=GetFileAttributesW(filename);

            if(attr==INVALID_FILE_ATTRIBUTES)
                return(false);

            if(attr&(FILE_ATTRIBUTE_DEVICE|FILE_ATTRIBUTE_DIRECTORY))    //不是个文件
                return(false);

            if(attr&FILE_ATTRIBUTE_OFFLINE)
                return(false);

            return (attr&FILE_ATTRIBUTE_READONLY)?false:true;
        }

        /**
         * 检测文件是否可执行
        * @param filename 文件名
         */
        bool FileCanExec(const OSString &filename)
        {
            int index = filename.FindRightChar('.');

            if (index == -1)return(false);

            if (index > filename.Length() - 4)
                return(false);

            const os_char *ext = filename.c_str() + index + 1;

            if (!ext)return(false);

            if (stricmp(ext, "exe") == 0)return(true);
            if (stricmp(ext, "com") == 0)return(true);
            if (stricmp(ext, "bat") == 0)return(true);
            if (stricmp(ext, "msi") == 0)return(true);
            if (stricmp(ext, "msp") == 0)return(true);

            return(false);
        }

        /**
        * 判断当前名称是否是一个目录
        * @param name 名称
        */
        bool IsDirectory(const os_char *name)
        {
            DWORD attrib = GetFileAttributesW(name);

            if (attrib == INVALID_FILE_ATTRIBUTES)
                return(false);

            return attrib&FILE_ATTRIBUTE_DIRECTORY;
        }

        bool MakeDirectory(const OSString &name)
        {
            if(::CreateDirectoryW(name,nullptr))return(true);

            LOG_PROBLEM(OS_TEXT("创建目录<")+name+OS_TEXT(">失败！"));
            return(false);
        }

        os_char *GetRootPath(os_char *str)
        {
            if(str[1]==OS_TEXT(':'))
                return str+3;

            return str;
        }

        /**
        * 删除一个子目录
        * @param name 目录名称
        * @return 目录是否删除成功
        */
        bool DeletePath(const OSString &name)
        {
            return(RemoveDirectoryW(name));
        }

        /**
        * 取得当前所在目录<br>
        */
        bool GetCurrentPath(OSString &path)
        {
            int len;
            u16char *dir;

            len=GetCurrentDirectoryW(0,nullptr);

            if(len==0)
                return(nullptr);

            dir=new u16char[len+1];

            if(GetCurrentDirectoryW(len,dir))
            {
                if(len==3&&dir[1]==OS_TEXT(':'))
                    len=2;        //如果是"C:\"这种情况，去掉"\"

                dir[len]=0;

                path.Set(dir,len,true);
                return(true);
            }

            delete[] dir;
            return(false);
        }

        /**
         * 取得当前程序完整路径名称
         */
        bool GetCurrentProgram(OSString &result)
        {
            os_char *path=new os_char[HGL_MAX_PATH];

            GetModuleFileNameW(nullptr,path,HGL_MAX_PATH);

            result = path;
            delete[] path;

            return(true);
        }

        EnumFileConfig *DefaultCreateSubConfig(struct EnumFileConfig *efc,const OSString &sub_folder_name)
        {
            OSString full_sub_folder_name;

            MergeFilename(full_sub_folder_name,efc->folder_name,sub_folder_name);

            return(new EnumFileConfig(efc,full_sub_folder_name));
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
                MergeFilename(full_findname,config->folder_name,config->find_name);
            }

            WIN32_FIND_DATAW FindFileData;
            HANDLE hFind;

            hFind = FindFirstFileW(full_findname, &FindFileData);
            if (hFind == INVALID_HANDLE_VALUE)
                return(-1);

            EnumFileConfig *sub_efc=nullptr;

            do
            {
                if(strcmp(FindFileData.cFileName,OS_TEXT("."))==0
                || strcmp(FindFileData.cFileName,OS_TEXT(".."))==0)
                {
                    continue;
                }

                if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
                {
                    if(config->proc_folder)
                    {
                        if(config->sub_folder)
                        {
                            sub_efc=config->CreateSubConfig(config,FindFileData.cFileName);

                            if(!sub_efc)
                                continue;

                            count+=EnumFile(sub_efc);
                        }
                    }
                    else
                        continue;
                }
                else
                {
                    if(!config->proc_file)continue;
                }

                count++;

                FileInfo fi;
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

                fi.can_read =true;
                fi.can_write=!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_READONLY);

                if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
                {
                    fi.is_file=false;
                    fi.is_directory=true;

                    if(config->cb_folder)
                        config->cb_folder(config,sub_efc,fi);

                    delete sub_efc;
                    sub_efc=nullptr;
                }
                else
                {
                    fi.is_file=true;
                    fi.is_directory=false;

                    if(config->cb_file)
                        config->cb_file(config,fi);
                }
            }
            while(FindNextFileW(hFind, &FindFileData));

            FindClose(hFind);

            return(count);
        }

        /**
        * 枚举当前计算机所有卷
        * @param data 用户自定义回传信息
        * @param func 回调函数
        * @param check_cd 检测光盘
        * @return 查找到的卷数量，-1表示失败
        */
        int EnumVolume(void *data,void (__cdecl *func)(void *,hgl::filesystem::VolumeInfo &),bool check_removable,bool check_remote,bool check_cd)
        {
            HANDLE handle;
            u16char volume_name[HGL_MAX_PATH+1];
            u16char path_name[HGL_MAX_PATH];
            int count=0;

            handle=FindFirstVolumeW(volume_name,HGL_MAX_PATH);

            if(handle==INVALID_HANDLE_VALUE)return(-1);

            do
            {
                hgl::filesystem::VolumeInfo vi;

                memset(&vi,0,sizeof(hgl::filesystem::VolumeInfo));

                DWORD length;

                GetVolumePathNamesForVolumeNameW(volume_name,path_name,HGL_MAX_PATH,&length);                //这个函数要win xp/2003才能用

                path_name[length]=0;

                strcpy(vi.name, HGL_MAX_PATH,volume_name);
                strcpy(vi.path, HGL_MAX_PATH,path_name);

                UINT type=GetDriveTypeW(path_name);

                if(type==DRIVE_REMOVABLE){if(!check_removable   )continue;vi.driver_type=hgl::filesystem::VolumeInfo::dtRemovable;}else
                if(type==DRIVE_FIXED    ){                                vi.driver_type=hgl::filesystem::VolumeInfo::dtFixed;    }else
                if(type==DRIVE_REMOTE   ){if(!check_remote      )continue;vi.driver_type=hgl::filesystem::VolumeInfo::dtRemote;   }else
                if(type==DRIVE_RAMDISK  ){                                vi.driver_type=hgl::filesystem::VolumeInfo::dtRamDisk;  }else
                if(type==DRIVE_CDROM    ){if(!check_cd          )continue;vi.driver_type=hgl::filesystem::VolumeInfo::dtCDROM;    }else
                    vi.driver_type=hgl::filesystem::VolumeInfo::dtNone;

                uint32 file_system_flags;

                if(GetVolumeInformationW(path_name,
                                        vi.volume_label,
                                        255,
                                        (unsigned long *)&vi.serial,
                                        (unsigned long *)&vi.filename_max_length,
                                        (unsigned long *)&file_system_flags,
                                        vi.file_system,
                                        255))
                {
                    vi.unicode=file_system_flags&FILE_UNICODE_ON_DISK;
                }
                else
                    LOG_PROBLEM(U16_TEXT("取得卷<") + UTF16String(path_name) + U16_TEXT(">信息失败！Windows错误编号: ") + UTF16String((uint)GetLastError()));

                if(GetDiskFreeSpaceExW( path_name,
                                        (ULARGE_INTEGER *)&vi.available_space,
                                        (ULARGE_INTEGER *)&vi.total_space,
                                        (ULARGE_INTEGER *)&vi.free_space))
                {
                    func(data,vi);
                }
                else
                    LOG_PROBLEM(U16_TEXT("取得驱动器<") + UTF16String(path_name) + U16_TEXT(">容量数据失败！"));

                count++;

            }while(FindNextVolumeW(handle,volume_name,HGL_MAX_PATH));

            FindVolumeClose(handle);

            return(count);
        }
    }//namespace filesystem
}//namespace hgl

