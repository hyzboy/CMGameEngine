#ifndef HGL_IO_FILE_INPUT_STREAM_INCLUDE
#define HGL_IO_FILE_INPUT_STREAM_INCLUDE

#include<hgl/io/InputStream.h>
#include<hgl/io/FileAccess.h>
namespace hgl
{
    namespace io
    {
        /**
        * 文件输入流类
        */
        class FileInputStream:public InputStream                                                    ///文件输入流类
        {
        protected:

            SharedPtr<FileAccess> file;                                                             ///<文件访问指针

        public:

            FileInputStream();
            FileInputStream(SharedPtr<FileAccess> &);
            virtual ~FileInputStream();

            virtual bool    Open(const OSString &);                                                 ///<打开文件
            virtual void    Close();                                                                ///<关闭文件输入流

            virtual int     GetFileHandle()const{return file->GetFileHandle();}

            virtual int64   Read(void *,int64);                                                     ///<读取数据
            virtual int64   Peek(void *,int64);                                                     ///<预读数据

            virtual bool    CanSeek()const;                                                         ///<是否可移动访问指针
            virtual bool    CanRestart()const;                                                      ///<是否可复位访问
            virtual bool    CanSize()const;                                                         ///<是否可访问文件长度
            virtual bool    CanPeek()const;                                                         ///<是否可预读数据

            virtual int64   Skip(int64);                                                            ///<跳过指定字节
            virtual int64   Tell()const;                                                            ///<取当前位置
            virtual int64   GetSize()const;                                                         ///<取得文件长度
            virtual bool    Restart();                                                              ///<复位访问指针
            virtual int64   Seek(int64,SeekOrigin=soBegin);                                         ///<移动访问指针

            virtual int64   Available()const;                                                       ///<剩下的可以不受阻塞访问的字节数

            virtual int64   Read(int64,void *,int64);                                               ///<在指定位置读取指定长度的数据
        };//class FileInputStream

        class OpenFileInputStream
        {
            FileInputStream *fis;

        public:

            OpenFileInputStream(const OSString &filename)
            {
                fis=new FileInputStream();

                if(!fis->Open(filename))
                {
                    delete fis;
                    fis=nullptr;
                }
            }

            ~OpenFileInputStream()
            {
                SAFE_CLEAR(fis);
            }

            const bool operator !(){return !fis;}
            operator FileInputStream *(){return fis;}
            FileInputStream *operator &(){return fis;}
            FileInputStream *operator ->(){return fis;}
        };//class OpenFileInputStream
    }//namespace io
}//namespace hgl
#endif//HGL_IO_FILE_INPUT_STREAM_INCLUDE
