#include<hgl/platform/SystemInfo.h>
#include<hgl/LogInfo.h>
#include<hgl/PlugIn.h>
#include<hgl/Other.h>
#include<hgl/File.h>
#include<wchar.h>
#include<shlobj.h>

namespace hgl
{
	bool GetTempPath(u16char *path,unsigned long s)
	{
		HKEY hKey;
		DWORD type;
		unsigned long size=1024;

		LONG result = RegOpenKeyEx(HKEY_CURRENT_USER,L"Environment",0,KEY_READ,&hKey);

		if(result==ERROR_SUCCESS)
		{
			u16char temp[1024];

			result = RegQueryValueEx( hKey,L"TEMP", NULL, &type, (LPBYTE)temp, &size );

			RegCloseKey(hKey);

			if(result==ERROR_SUCCESS)
			{
				ExpandEnvironmentStrings(temp,path,s);
				return(true);
			}
		}

		return(false);
	}

	bool GetCMGDKPath(UTF16String &cmgdk_path)
	{
		HKEY hKey;
		DWORD type;

		u16char path[HGL_MAX_PATH];
		unsigned long size=HGL_MAX_PATH;

		LONG result = RegOpenKeyEx(HKEY_CURRENT_USER,L"Environment",0,KEY_READ,&hKey);

		if(result==ERROR_SUCCESS)
		{
			result = RegQueryValueEx( hKey,L"CMGDK", NULL, &type, (LPBYTE)path, &size );

			RegCloseKey(hKey);

			cmgdk_path=path;

			return(result==ERROR_SUCCESS);
		}

		return(false);
	}

	void GetOSPath(CMGDKPATH &cp)
	{
		u16char path[HGL_MAX_PATH];

		#define GET_FOLDER(str,attrib)	SHGetFolderPath(nullptr,attrib,nullptr,0,path);	\
										cp.str=path;

		GET_FOLDER(	os			,CSIDL_WINDOWS	);
		GET_FOLDER(	library		,CSIDL_SYSTEM	);
		GET_FOLDER(	osfont		,CSIDL_FONTS	);

		hgl::GetTempPath(cp.temp,HGL_MAX_PATH);

		GET_FOLDER( common_data	,CSIDL_COMMON_APPDATA);
		GET_FOLDER( local_data	,CSIDL_LOCAL_APPDATA);

		GET_FOLDER(	mydata		,CSIDL_APPDATA);
		GET_FOLDER(	myprogram	,CSIDL_STARTMENU);
		GET_FOLDER(	mydesktop	,CSIDL_DESKTOP);

		#undef GET_FOLDER
	}
}//namespace hgl
