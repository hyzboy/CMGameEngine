#include<hgl/io/FileAccess.h>
#include<hgl/LogInfo.h>
#include<io.h>
#include<share.h>
#include<fcntl.h>

namespace hgl
{
    namespace io
    {
        bool FileAccess::CreateTemp()
        {
            const uint buf_size=HGL_MAX_PATH;

            u16char PathBuffer[buf_size];
            u16char TempName[buf_size];

            GetTempPathW(buf_size,PathBuffer);

            GetTempFileNameW(PathBuffer,L"NEW",0,TempName);

            return Open(TempName,fomCreate);
        }

        int OpenFile(const os_char *fn,FileOpenMode fom)
        {
            int fp;

            errno_t result;

            if(fom==fomCreate        )result=_wsopen_s(&fp,fn,_O_BINARY|_O_WRONLY|_O_CREAT            ,_SH_DENYNO,S_IREAD|_S_IWRITE);else
            if(fom==fomCreateTrunc     )result=_wsopen_s(&fp,fn,_O_BINARY|_O_WRONLY|_O_CREAT|_O_TRUNC    ,_SH_DENYNO,S_IREAD|_S_IWRITE);else
            if(fom==fomOnlyRead        )result=_wsopen_s(&fp,fn,_O_BINARY|_O_RDONLY                    ,_SH_DENYNO,S_IREAD|_S_IWRITE);else
            if(fom==fomOnlyWrite    )result=_wsopen_s(&fp,fn,_O_BINARY|_O_WRONLY                    ,_SH_DENYNO,S_IREAD|_S_IWRITE);else
            if(fom==fomReadWrite    )result=_wsopen_s(&fp,fn,_O_BINARY|_O_RDWR                        ,_SH_DENYNO,S_IREAD|_S_IWRITE);else
            if(fom==fomAppend        )result=_wsopen_s(&fp,fn,_O_BINARY|_O_APPEND                    ,_SH_DENYNO,S_IREAD|_S_IWRITE);else
                RETURN_ERROR(-1);

            return(fp);
        }

        void CloseFile(int fp)
        {
            _close(fp);
        }

        int64 FileAccess::Read(int64 offset,void *buf,int64 size)
        {
            if(!CanRead())return(-1);

            if(_lseeki64(fp,offset,soBegin)==offset)
                return _read(fp,buf,size);
            else
                return -1;
        }

        int64 FileAccess::Write(int64 offset,const void *buf,int64 size)
        {
            if(!CanWrite())return(-1);

            if(_lseeki64(fp,offset,soBegin)==offset)
                return _write(fp,buf,size);
            else
                return -1;
        }
    }//namespace io
}//namespace hgl
