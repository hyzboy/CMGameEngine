#include<hgl/filesystem/EnumVolume.h>
#include<hgl/type/BaseString.h>
#include<hgl/LogInfo.h>
#include<windows.h>

namespace hgl
{
    namespace filesystem
    {
        int EnumVolume(List<VolumeInfo> &vi_list,const VolumeCheckConfig &cfg)
        {
            if(cfg.isErrorConfig())
                return(-1);

            HANDLE handle;
            u16char volume_name[HGL_MAX_PATH+1];
            u16char path_name[HGL_MAX_PATH];
            int count=0;

            handle=FindFirstVolumeW(volume_name,HGL_MAX_PATH);

            if(handle==INVALID_HANDLE_VALUE)return(-2);

            do
            {
                hgl::filesystem::VolumeInfo vi;

                memset(&vi,0,sizeof(hgl::filesystem::VolumeInfo));

                DWORD length;

                GetVolumePathNamesForVolumeNameW(volume_name,path_name,HGL_MAX_PATH,&length);                //这个函数要win xp/2003才能用

                path_name[length]=0;

                hgl::strcpy(vi.name,HGL_MAX_PATH,volume_name);
                hgl::strcpy(vi.path,HGL_MAX_PATH,path_name);

                UINT type=GetDriveTypeW(path_name);

                if(type==DRIVE_REMOVABLE){  if(!cfg.removable   )continue;vi.driver_type=hgl::filesystem::VolumeInfo::dtRemovable;  }else
                if(type==DRIVE_FIXED    ){  if(!cfg.fixed       )continue;vi.driver_type=hgl::filesystem::VolumeInfo::dtFixed;      }else
                if(type==DRIVE_REMOTE   ){  if(!cfg.remote      )continue;vi.driver_type=hgl::filesystem::VolumeInfo::dtRemote;     }else
                if(type==DRIVE_RAMDISK  ){  if(!cfg.ram_disk    )continue;vi.driver_type=hgl::filesystem::VolumeInfo::dtRamDisk;    }else
                if(type==DRIVE_CDROM    ){  if(!cfg.cdrom       )continue;vi.driver_type=hgl::filesystem::VolumeInfo::dtCDROM;      }else
                {
                    if(cfg.unknow)vi.driver_type=hgl::filesystem::VolumeInfo::dtNone;
                    continue;
                }

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
                {
                    LOG_PROBLEM(U16_TEXT("Get <")+UTF16String(path_name)+U16_TEXT("> info failed！Windows error code: ")+UTF16String((uint)GetLastError()));
                }

                if(GetDiskFreeSpaceExW(path_name,
                    (ULARGE_INTEGER *)&vi.available_space,
                    (ULARGE_INTEGER *)&vi.total_space,
                    (ULARGE_INTEGER *)&vi.free_space))
                {
                    vi_list.Add(vi);
                }
                else
                {
                    LOG_PROBLEM(U16_TEXT("Get disk free space <")+UTF16String(path_name)+U16_TEXT("> data failed,Windows error code: ")+UTF16String((uint)GetLastError()));
                }

                count++;
            } while(FindNextVolumeW(handle,volume_name,HGL_MAX_PATH));

            FindVolumeClose(handle);

            return(count);
        }
    }//namespace filesystem
}//namespace hgl
