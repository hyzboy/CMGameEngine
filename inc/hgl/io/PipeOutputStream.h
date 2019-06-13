#ifndef HGL_IO_PIPE_OUTPUT_STREAM_INCLUDE
#define HGL_IO_PIPE_OUTPUT_STREAM_INCLUDE

#include<hgl/io/OutputStream.h>
namespace hgl
{
    namespace io
    {
        /**
        * 管道输出流
        */
        class PipeOutputStream:public OutputStream
        {
            pipe_ptr pipe_fd;

        public:

            PipeOutputStream(){pipe_fd=PIPE_NULL;}
            PipeOutputStream(pipe_ptr fd){pipe_fd=fd;}
            ~PipeOutputStream(){Close();}

            void    Init(pipe_ptr fd){pipe_fd=fd;}
            void    Close();                                                                        ///<关闭输出流

            int64   Write(const void *,int64);                                                      ///<写入数据
            int64   WriteFully(const void *buf,int64 buf_size);                                     ///<充分写入,保证完整写入指定长度的数据

            bool    CanRestart()const{return false;}                                                ///<是否可以复位
            bool    CanSeek()const{return false;}                                                   ///<是否可以定位
            bool    CanSize()const{return false;};                                                  ///<是否可以取得尺寸

            bool    Restart(){return false;}                                                        ///<复位访问指针
            int64   Seek(int64,SeekOrigin=soBegin){return -1;}                                      ///<移动访问指针
            int64   Tell()const{return -1;}                                                         ///<返回当前访问位置
            int64   GetSize()const{return -1;}                                                      ///<取得流长度
            int64   Available()const{return -1;}                                                    ///<剩下的可以不受阻塞写入的字节数
        };//class OutputStream
    }//namespace io
}//namespace hgl
#endif//HGL_IO_PIPE_OUTPUT_STREAM_INCLUDE
