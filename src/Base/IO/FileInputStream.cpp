#include<hgl/io/FileInputStream.h>
namespace hgl
{
    namespace io
    {
        FileInputStream::FileInputStream()
        {
        }

        FileInputStream::FileInputStream(SharedPtr<FileAccess> &fa)
        {
            file=fa;
        }

        FileInputStream::~FileInputStream()
        {
            Close();
        }

        bool FileInputStream::Open(const OSString &filename)
        {
            Close();

            if(filename.IsEmpty())
                return(false);

            file=new FileAccess();

            if(!file->Open(filename,fomOnlyRead))
            {
                file=nullptr;
                return(false);
            }

            return(true);
        }

        void FileInputStream::Close()
        {
            if(file)
            {
                file->Close();
                file=nullptr;
            }
        }

        int64    FileInputStream::Read        (void *buf,int64 size)            {return file?file->Read(buf,size):-1;}
        int64    FileInputStream::Peek        (void *buf,int64 size)            {return file?file->Peek(buf,size):-1;}

        bool    FileInputStream::CanSeek    ()const                            {return file?file->CanSeek():false;}
        bool    FileInputStream::CanRestart    ()const                            {return file?file->CanRestart():false;}
        bool    FileInputStream::CanSize    ()const                            {return file?file->CanSize():false;}
        bool    FileInputStream::CanPeek    ()const                            {return file?file->CanPeek():false;}

        int64    FileInputStream::Skip        (int64 bytes)                    {return file?file->Seek(bytes,soCurrent):-1;}
        int64    FileInputStream::Tell        ()const                            {return file?file->Tell():-1;}
        int64    FileInputStream::GetSize    ()const                            {return file?file->GetSize():-1;}
        bool    FileInputStream::Restart    ()                                {return file?file->Restart():false;}
        int64    FileInputStream::Seek        (int64 off,SeekOrigin so)        {return file?file->Seek(off,so):-1;}

        int64    FileInputStream::Available    ()const                            {return file?file->AvailableRead():-1;}

        int64    FileInputStream::Read        (int64 off,void *buf,int64 size){return file?file->Read(off,buf,size):-1;}
    }//namespace io
}//namespace hgl
