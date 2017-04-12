#include<hgl/network/SocketInputStream.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/network/TCPSocket.h>
#include<hgl/MemBlock.h>
#include<hgl/LogInfo.h>
namespace hgl
{
    namespace network
    {
        bool Read(io::DataInputStream *dis,sockaddr_in &addr)
        {
            if(dis->ReadUint8((uint8 *)&(addr.sin_addr.s_addr),4)!=4)
                return(false);

            return dis->ReadUint16(addr.sin_port);
        }
    }//namespace network

    namespace network
    {
        SocketInputStream::SocketInputStream(int s)
        {
//            LOG_INFO(OS_TEXT("SocketInputStream::SocketInputStream(")+OSString(s)+OS_TEXT(")"));

            SetSocket(s);
            mb=new MemBlock<char>();
        }

        SocketInputStream::~SocketInputStream()
        {
//            LOG_INFO(OS_TEXT("SocketInputStream::~SocketInputStream(")+OSString(sock)+OS_TEXT(")"));

            SAFE_CLEAR(mb);
        }

        /**
        * 从socket中读取指定的字节数
        * @param buf 数据保存缓冲区
        * @param size 预想读取的字节数
        * @return 成功读取的字节数
        * @return -1 失败
        */
        int64 SocketInputStream::Read(void *buf,int64 size)
        {
            if(sock==-1)return(-1);
			if(!buf||size<=0)return(-1);

            const int64 result=recv(sock,(char *)buf,size,0);

            if(result>0)
            {
                total+=result;

//                LOG_INFO(OS_TEXT("Socket ")+OSString(sock)+OS_TEXT(" recv ")+OSString(size)+OS_TEXT(" bytes ok,result ")+OSString(result)+OS_TEXT(" total recv ")+OSString(total)+OS_TEXT(" bytes."));
            }
            else
            {
                LOG_INFO(OS_TEXT("Socket ")+OSString(sock)+OS_TEXT(" recv ")+OSString(size)+OS_TEXT(" bytes failed,result ")+OSString(result));
            }

            return(result);
        }

        /**
        * 从socket中读取指定的字节数，但不从缓存队列中删除
        * @param buf 数据保存缓冲区
        * @param size 预想读取的字节数
        * @return 成功读取的字节数
        * @return -1 失败
        */
        int64 SocketInputStream::Peek(void *buf,int64 size)
        {
            if(sock==-1)return(-1);
			if(!buf||size<=0)return(-1);

            return recv(sock,(char *)buf,size,MSG_PEEK);
        }

        /**
        * 从socket中充分读取指定的字节数，无视超时，直接读满指定字节数为止
        * @param buf 数据保存缓冲区
        * @param size 预想读取的字节数
        * @return 成功读取的字节数
        * @return -1 失败
        */
        int64 SocketInputStream::ReadFully(void *buf,int64 size)
        {
            if(sock==-1)return(-1);
			if(!buf||size<=0)return(-1);

            bool to_first=true;
            int err;
            const os_char *err_str;
            char *p=(char *)buf;

#if HGL_OS == HGL_OS_Windows
            int result = 0;
            int left_bytes = size;
#else
            ssize_t result = 0;
            size_t left_bytes = size;
#endif//HGL_OS == HGL_OS_Windows

//             const double start_time=GetDoubleTime();

            while(left_bytes>0)
            {
                //如果最后一个参数使用MSG_WAITALL，则无论是否阻塞模式，都会永远阻塞
                //使用0则会得出一个11号nseTryAgain错误
                result=recv(sock,p,left_bytes,0);            //似乎windows 2003才开始支持MSG_WAITALL

                if(result==0)                               //linux下返回0即为对方断开连接,win/bsd下未验证
                {
                    sock=-1;
                    break;
                }

                if(result<0)
                {
                    err=GetLastSocketError();

//                     std::cerr<<"SocketInputStream::ReadFully error,Socket:"<<sock<<",error code="<<err<<std::endl;

                    if(err==0                //没有错误
                    ||err==4                //Interrupted system call(比如ctrl+c,一般DEBUG下才有)
                    )
                        continue;

                    if(err==nseTryAgain)    //资源临时不可用，仅代表没数据，并不是出错的意思
                        break;

                     if(err==nseTimeOut)        //超时
                     {
//                         if(to_first)
//                         {
//                             to_first=false;
//                             continue;
//                         }

                        LOG_ERROR(OS_TEXT("SocketInputStream::ReadFully TimeOut,Socket:")+OSString(sock));
                    }

                    err_str=GetSocketString(err);
                    if(err_str)
                    {
                        LOG_ERROR(OS_TEXT("SocketInputStream::ReadFully error,Socket:")+OSString(sock)+OS_TEXT(",error code=")+OSString(err)+OS_TEXT(":")+OSString(err_str));
                    }
                    else
                    {
                        LOG_ERROR(OS_TEXT("SocketInputStream::ReadFully error,Socket:")+OSString(sock)+OS_TEXT(",error code=")+OSString(err));
                    }

                    sock=-1;
                    break;
                }
                else
                {
                    p+=result;
                    left_bytes-=result;
                }
            }

//             const double end_time=GetDoubleTime();

//             LOG_INFO(OS_TEXT("SocketInputStream::ReadFully() time: ")+OSString(end_time-start_time));

//             #ifdef _DEBUG
//             result=(p-(char *)buf);
//             total+=result;
//
//             LOG_INFO(OS_TEXT("Socket ")+OSString(sock)+OS_TEXT(" recv ")+OSString(result)+OS_TEXT(" bytes,total recv ")+OSString(total)+OS_TEXT(" bytes."));
//            #else
            total+=(p-(char *)buf);
//             #endif//_DEBUG

            return(p-(char *)buf);
        }

        int64 SocketInputStream::Skip(int64 n)
        {
            if(n<=0)return(n);

            mb->SetLength(n);

            return ReadFully(mb->data(),n);
        }
    }//namespace network
}//namespace hgl
