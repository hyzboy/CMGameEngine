#include<hgl/io/FileSystem.h>
#include<hgl/LogInfo.h>
#include<hgl/io/FileInputStream.h>
#include<hgl/io/FileOutputStream.h>

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
        bool FileCopy(const OSString &targetname,const OSString &sourcename)
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
        bool FileMove(const OSString &targetname,const OSString &sourcename)
        {
            return(::MoveFileW(sourcename,targetname));
        }

        /**
        * 修改文件名
        * @param oldname 旧的文件名
        * @param newname 新的文件名
        * @return 文件名是否修改成功
        */
        bool FileRename(const OSString &newname,const OSString &oldname)
        {
            return(::MoveFileW(oldname,newname));
        }

        /**
        * 确认文件是否存在
        * @param filename 要查找的文件名称
        * @return 这个文件是否存在
        */
        bool FileExist(const OSString &filename)
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
