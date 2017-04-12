#include<hgl/network/SocketOutputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/network/TCPSocket.h>
#include<hgl/LogInfo.h>
namespace hgl
{
    namespace network
    {
//         bool Write(io::DataOutputStream *dos,const sockaddr_in &addr)
//         {
//         #if (HGL_COMPILER == HGL_COMPILER_GNU)||(HGL_COMPILER == HGL_COMPILER_LLVM)
//             if(dos->WriteUint8((uint8 *)&(addr.sin_addr.s_addr),4)!=4)return(false);
//         #else
//             #if (HGL_OS == HGL_OS_Windows)
//                 if(dos->WriteUint8((uint8 *)&(addr.sin_addr.S_addr),4)!=4)return(false);
//             #endif//
//         #endif//
//
//             return dos->WriteUint16(addr.sin_port);
//         }
    }//namespace network

    namespace network
    {
        /**
        * 向socket中写入指定的字节数
        * @param buf 数据缓冲区
        * @param size 预想写入的字节数
        * @return 成功写入的字节数
        * @return -1 失败
        */
        int64 SocketOutputStream::Write(const void *buf,int64 size)
        {
            if(sock==-1)return(-1);
			if(!buf||size<=0)return(-1);

            const int64 result=send(sock,(char *)buf,size,0);

            if(result>0)
            {
                total+=result;

//                LOG_INFO(OS_TEXT("Socket ")+OSString(sock)+OS_TEXT(" send ")+OSString(size)+OS_TEXT(" bytes ok,result ")+OSString(result)+OS_TEXT(" total send ")+OSString(total)+OS_TEXT(" bytes."));
            }
            else
            {
                LOG_INFO(OS_TEXT("Socket ")+OSString(sock)+OS_TEXT(" send ")+OSString(size)+OS_TEXT(" bytes failed,result ")+OSString(result));
            }

            return(result);
        }

        /**
        * 向socket中充分写入指定的字节数，无视超时，直接写完指定字节数为止
        * @param buf 数据缓冲区
        * @param size 预想写入的字节数
        * @return 成功写入的字节数
        * @return -1 失败
        */
        int64 SocketOutputStream::WriteFully(const void *buf,int64 size)
        {
            if(sock==-1)return(-1);
			if(!buf||size<=0)return(-1);

            int err;
            const os_char *err_str;
            char *p=(char *)buf;

//             const double start_time=GetDoubleTime();

#if HGL_OS == HGL_OS_Windows
            int result = 0;
            int left_bytes = size;
#else
            ssize_t result = 0;
            size_t left_bytes = size;
#endif//HGL_OS == HGL_OS_Windows

            while(left_bytes>0)
            {
                result=send(sock,p,left_bytes,0);

//                 LOG_ERROR(OS_TEXT("socket send  result, left_bytes:  ")+OSString(sock)+OSString(" , ")+OSString((int)result)+OSString(" , ")+OSString((int)left_bytes));
                if(result==0)
                {
                    sock=-1;
                    break;
                }

                if(result<0)
                {
                    err=GetLastSocketError();

                    if(err==0
                     ||err==nseTimeOut
                     ||err==nseTryAgain
                     ||err==nseInt
                    )
                        continue;

                    if(err==nseBrokenPipe)
                    {
                        sock=-1;
                        break;
                    }

                    err_str=GetSocketString(err);
                    if(err_str)
                    {
                        LOG_ERROR(OS_TEXT("SocketOutputStream::WriteFully error,Socket:")+OSString(sock)+OS_TEXT(",error code=")+OSString(err)+OS_TEXT(":")+OSString(err_str));
                    }
                    else
                    {
                        LOG_ERROR(OS_TEXT("SocketOutputStream::WriteFully error,Socket:")+OSString(sock)+OS_TEXT(",error code=")+OSString(err));
                    }
                    sock=-1;
                    break;
                }

                p+=result;
                left_bytes-=result;
            }

//             const double end_time=GetDoubleTime();

//             LOG_INFO(OS_TEXT("SocketOutputStream::WriteFully() time: ")+OSString(end_time-start_time));

            total+=(p-(char *)buf);

            return(p-(char *)buf);
        }
    }//namespace network
}//namespace hgl
