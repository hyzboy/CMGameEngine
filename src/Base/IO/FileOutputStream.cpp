#include<hgl/io/FileOutputStream.h>
namespace hgl
{
    namespace io
    {
        FileOutputStream::FileOutputStream()
        {
        }

        FileOutputStream::FileOutputStream(SharedPtr<FileAccess> &fa)
        {
            file=fa;
        }

        FileOutputStream::~FileOutputStream()
        {
            Close();
        }

        bool FileOutputStream::OpenFile(const OSString &filename,FileOpenMode mode)
        {
            Close();

            if(filename.IsEmpty())
                return(false);

            file=new FileAccess();

            if(!file->Open(filename,mode))
            {
                file=nullptr;
                return(false);
            }

            return(true);
        }

        void FileOutputStream::Close()
        {
            if(file)
            {
                file->CloseWrite();
                file=nullptr;
            }
        }

        int64   FileOutputStream::Write     (const void *buf,int64 size)            {return file?file->Write(buf,size):-1;}

        bool    FileOutputStream::CanSeek   ()const                                 {return file?file->CanSeek():false;}
        bool    FileOutputStream::CanRestart()const                                 {return file?file->CanRestart():false;}
        bool    FileOutputStream::CanSize   ()const                                 {return file?file->CanSize():false;}

        int64   FileOutputStream::Tell      ()const                                 {return file?file->Tell():-1;}
        int64   FileOutputStream::GetSize   ()const                                 {return file?file->GetSize():-1;}
        bool    FileOutputStream::Restart   ()                                      {return file?file->Restart():false;}
        int64   FileOutputStream::Seek      (int64 off,SeekOrigin so)               {return file?file->Seek(off,so):-1;}

        int64   FileOutputStream::Write     (int64 off,const void *buf,int64 size)  {return file?file->Write(off,buf,size):-1;}
    }//namespace io
}//namespace hgl
