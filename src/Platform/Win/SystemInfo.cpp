#include<hgl/platform/SystemInfo.h>
#include<hgl/platform/ConsoleSystemInitInfo.h>
#include<hgl/LogInfo.h>
#include<hgl/PlugIn.h>
#include<hgl/FileSystem.h>
#include<wchar.h>
#include<shlobj.h>

namespace hgl
{
    bool GetTempPath(wchar_t *path,unsigned long s)
    {
        HKEY hKey;
        DWORD type;

        LONG result = RegOpenKeyExW(HKEY_CURRENT_USER,L"Environment",0,KEY_READ,&hKey);

        if(result==ERROR_SUCCESS)
        {
            wchar_t temp[HGL_MAX_PATH];
            DWORD size=HGL_MAX_PATH;

            result = RegQueryValueExW( hKey,L"TEMP", NULL, &type, (LPBYTE)temp, &size );

            RegCloseKey(hKey);

            if(result==ERROR_SUCCESS)
            {
                ExpandEnvironmentStringsW(temp,path,s);
                return(true);
            }
        }

        return(false);
    }

    bool GetCMGDKPath(WideString &cmgdk_path)
    {
        HKEY hKey;
        DWORD type;

        LONG result = RegOpenKeyExW(HKEY_CURRENT_USER,L"Environment",0,KEY_READ,&hKey);

        if(result==ERROR_SUCCESS)
        {
            wchar_t path[HGL_MAX_PATH];
            DWORD size=HGL_MAX_PATH;

            result = RegQueryValueExW( hKey,L"CMGDK", NULL, &type, (LPBYTE)path, &size );

            RegCloseKey(hKey);

            cmgdk_path=path;

            return(result==ERROR_SUCCESS);
        }

        return(false);
    }

    void GetOSPath(CMGDKPATH &cp)
    {
        wchar_t path[HGL_MAX_PATH];

        #define GET_FOLDER(str,attrib)    SHGetFolderPathW(nullptr,attrib,nullptr,0,path);    \
                                        cp.str=path;

        GET_FOLDER(os           ,CSIDL_WINDOWS          );
        GET_FOLDER(library      ,CSIDL_SYSTEM           );
        GET_FOLDER(osfont       ,CSIDL_FONTS            );

        hgl::GetTempPath(cp.temp,HGL_MAX_PATH);

        GET_FOLDER(common_data  ,CSIDL_COMMON_APPDATA   );
        GET_FOLDER(local_data   ,CSIDL_LOCAL_APPDATA    );

        GET_FOLDER(mydata       ,CSIDL_APPDATA          );
        GET_FOLDER(myprogram    ,CSIDL_STARTMENU        );
        GET_FOLDER(mydesktop    ,CSIDL_DESKTOPDIRECTORY );
        
        GET_FOLDER(desktop      ,CSIDL_COMMON_DESKTOPDIRECTORY );

        #undef GET_FOLDER
    }

    bool InitOSupport(ConsoleSystemInitInfo *sii)
    {
        if(sii->CheckDebugger&&IsDebuggerPresent())
        {
            LOG_ERROR(OS_TEXT("本程序不能运行在调试模式下!"));
            return(false);
        }

        return(true);
    }
}//namespace hgl
