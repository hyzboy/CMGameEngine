#include"Hac4.h"
#include<hgl/LogInfo.h>
#include<hgl/Compress.h>
#include<hgl/io/MemoryInputStream.h>
#include<hgl/io/PartStream.h>
#include<hgl/io/FileInputStream.h>

/*
 * HAC4相较HAC3改变：
 *
 * 1.文件位置索引改为64位
 * 2.文件索引数据改为可加密或压缩
 */

namespace hgl
{
    HAC4::HAC4(io::DataInputStream *fp)
    {
        RootFolder=new HacFolder<Hac4File>;
        RootFolder->FolderName=u"root";

        uint64 FileEnd;

        fp->Seek(-8,hgl::soEnd);
        fp->ReadUint32(FileEnd);
        fp->Position=FileEnd;

        TotalSize=0;

        LoadFolder(stream=fp,RootFolder);
    }

    HAC4::~HAC4()
    {
        CloseThread();

        delete RootFolder;
        delete stream;
    }

    void HAC4::LoadFolder(io::DataInputStream *fp,HacFolder<Hac4File> *folder)
    {
        int n;
        Hac4File *file;
        UTF16String folder_name;
        u16char ca_name[33];

        fp->ReadUTF16LEString(folder->FolderName);
        folder->FolderName.ToLowerCase();

        fp->ReadInt32(n);           //子目录数量
        while(n--)
        {
            HacFolder<Hac4File> *sub_folder=new HacFolder<Hac4File>;

            LoadFolder(fp,sub_folder);

            folder->Folder.Add(sub_folder->FolderName,sub_folder);
        }

        fp->ReadInt32(n);           //文件数量
        while(n--)
        {
            file=folder->File.Append();

            fp->ReadUTF16LEString(  file->FileName      );  file->FileName.LowerCase();
            fp->ReadUTF16LEString(  file->CompressName  );
            fp->ReadInt64(          file->FileSize      );
            fp->ReadInt64(          file->CompressSize  );
            fp->ReadFully(          file->md5           ,MD5Code::size());
            fp->ReadInt64(          file->Offset        );

            TotalSize+=file->FileSize;
        }
    }

    bool HAC4::LoadFilePart(void *file,int64 start,int64 length,void *data)
    {
        if(!file||!data)return(false);

        Hac4File *fm=(Hac4File *)file;

        if(start==0&&length==0)
            length=fm->FileSize;

        if(start+length>fm->FileSize)
            return(false);

        if(fm->ca<=caNone||fm->ca>=caEnd)
        {
            stream->Position=fm->Offset+start;
            stream->Read(data,length);

            return(true);
        }
        else
        {
            if(start==0&&length==fm->FileSize)
            {
                char *compress_data=new char[fm->CompressSize];

                stream->Position=fm->Offset;
                stream->Read(compress_data,fm->CompressSize);

                if(Decompress(fm->ca,compress_data,fm->CompressSize,data,fm->FileSize)!=fm->FileSize)
                {
                    PutError(u"解压文件<%s>失败，使用的是<%s>压缩算法！",fm->FileName.wc_str(),CAToName(fm->ca).wc_str());

                    delete[] compress_data;

                    return(false);
                }

                delete[] compress_data;

                return(true);
            }
            else
            {
                PutError(u"压缩的文件不能只读取一部分<%s>,<%d,%d>",fm->FileName.wc_str(),start,length);
                return(false);
            }
        }
    }

    io::InputStream *HAC4::LoadFileFrom(void *folder,const UTF16String &filename,bool load_to_memory)
    {
//      PutInfo(u"开始在HAC包中查找文件<%s>",filename);

        Hac4File *file=((HacFolder<Hac4File> *)folder)->FindFile(filename);

        if(file)
        {
//          PutInfo(u"在HAC包中成功找到文件<%s>",filename);

            if(file->ca<=caNone||file->ca>=caEnd)
            {
                if(load_to_memory)
                {
                    MemoryInputStream *ms=new MemoryInputStream(file->FileSize,true);

                    stream->Position=file->Offset;
                    stream->Read(ms->Data,file->FileSize);

                    return(ms);
                }
                else
                    return(new PartStream(stream,file->Offset,file->FileSize));
            }
            else
            {
                char *data=new char[file->CompressSize];
                MemStream *ms=new MemStream(file->FileSize,true);

                stream->Position=file->Offset;
                stream->Read(data,file->CompressSize);

                if(Decompress(file->ca,data,file->CompressSize,ms->Data,file->FileSize)!=file->FileSize)
                {
                    PutError(u"解压文件<%s>失败，使用的是<%s>压缩算法！",filename,CAToName(file->ca).wc_str());

                    delete[] data;
                    delete ms;
                    return(nullptr);
                }

                delete[] data;
                return(ms);
            }
        }
        else
        {
            ErrorHint(u"没有查到所需的文件<%s>",filename);

            return(nullptr);
        }
    }

    bool HAC4::LoadFileFrom(void *folder,const UTF16String &filename,void **data,int64 *size)
    {
        Hac4File *file=((HacFolder<Hac4File> *)folder)->FindFile(filename);

        if(file)
        {
            if(file->ca<=caNone||file->ca>=caEnd)
            {
                *data=new char[*size=file->FileSize];

                stream->Position=file->Offset;
                stream->Read(*data,file->FileSize);

                return(true);
            }
            else
            {
                char *compress_data=new char[file->CompressSize];
                *data=new char[*size=file->FileSize];

                stream->Position=file->Offset;
                stream->Read(compress_data,file->CompressSize);

                if(Decompress(file->ca,compress_data,file->CompressSize,*data,file->FileSize)!=file->FileSize)
                {
                    PutError(u"解压文件<%s>失败，使用的是<%s>压缩算法！",filename,CAToName(file->ca).wc_str());

                    delete[] *data;
                    delete[] compress_data;
                    return(nullptr);
                }

                delete[] compress_data;
                return(true);
            }
        }
        else
            ErrorHint(u"没有查到所需的文件<%s>",filename);

        return(false);
    }
}//namespace hgl
