#include<hgl/network/Socket.h>
#include<hgl/LogInfo.h>
#include<time.h>
#include<iostream>

#if HGL_OS != HGL_OS_Windows
#include<netinet/tcp.h>

#if HGL_OS == HGL_OS_Solaris
    #include<sys/filio.h>
#endif//HGL_OS == HGL_OS_Solaris
#endif//HGL_OS != HGL_OS_Windows

//setsockopt函数的Windows下使用参数格式请参见http://msdn.microsoft.com/en-us/library/windows/desktop/ms740476(v=vs.85).aspx

namespace hgl
{
    void SetTimeVal(timeval &tv,const double t_sec);

    namespace network
    {
#if HGL_OS == HGL_OS_Windows
    namespace
    {
        static bool winsocket_init=false;
    }//namespace

    bool InitWinSocket()
    {
        if(winsocket_init)return(true);

        WSADATA wsa;

        winsocket_init=(WSAStartup(MAKEWORD(2,2),&wsa)==NO_ERROR);

        return(winsocket_init);
    }
#endif//HGL_OS == HGL_OS_Windows

//        static atom_int socket_count=0;

        Socket::Socket()
        {
            ThisSocket=-1;

            socket_domain=-1;
            socket_type=-1;
            socket_protocols=-1;

//            LOG_INFO(u8"Socket Count ++: "+UTF8String(++socket_count));
        }

        Socket::~Socket()
        {
            CloseSocket();

//            LOG_INFO(u8"Socket Count --: "+UTF8String(--socket_count));
        }

        bool Socket::CreateSocket(int d,int t,int p)
        {
            #if HGL_OS == HGL_OS_Windows
            if(!InitWinSocket())
                RETURN_FALSE;
            #endif//HGL_OS == HGL_OS_Windows

            int s=socket(d,t,p);

            if(s<0)
            {
                LOG_ERROR(OS_TEXT("Socket::CreateSocket(domain=")+OSString(d)+
                            OS_TEXT(",type=")+OSString(t)+
                            OS_TEXT(",protocol=")+OSString(p)+
                            OS_TEXT(") return ")+OSString(s)+
                            OS_TEXT("; errno ")+OSString(GetLastSocketError()));

                RETURN_FALSE;
            }

            if(p==IPPROTO_UDP){LOG_INFO(OS_TEXT("Create UDP Socket OK: ")+OSString(s));}else
            if(p==IPPROTO_TCP){LOG_INFO(OS_TEXT("Create TCP Socket OK: ")+OSString(s));}else
#if HGL_OS != HGL_OS_Windows
            if(p==IPPROTO_SCTP){LOG_INFO(OS_TEXT("Create SCTP Socket OK: ")+OSString(s));}else
#endif//HGL_OS != HGL_OS_Windows
            {
                LOG_INFO(OS_TEXT("Create Protocol[")+OSString(p)+OS_TEXT("] Socket OK: ")+OSString(s));
            }

            socket_domain=d;
            socket_type=t;
            socket_protocols=p;

            ThisSocket=s;
            return(true);
        }

        /**
        * 关闭连接
        */
        void Socket::CloseSocket()
        {
            if(ThisSocket==-1)
                return;

            hgl::CloseSocket(ThisSocket);
            ThisSocket=-1;
        }

        struct SocketErrorMessage   //socket错误信息
        {
            int number;             //编号
            os_char recountal[256]; //详细叙述
        };

        const SocketErrorMessage tab[]=
        {
            {0,     OS_TEXT("没有错误")},

#if HGL_OS != HGL_OS_Windows
//             //errno-base.h

            {EPERM        ,OS_TEXT("Operation not permitted")},
            {ENOENT        ,OS_TEXT("No such file or directory")},
            {ESRCH        ,OS_TEXT("No such process")},
            {EINTR        ,OS_TEXT("Interrupted system call")},
            {EIO        ,OS_TEXT("I/O error")},
            {ENXIO        ,OS_TEXT("No such device or address")},
            {E2BIG        ,OS_TEXT("Argument list too long")},
            {ENOEXEC    ,OS_TEXT("Exec format error")},
            {EBADF        ,OS_TEXT("Bad file number")},
            {ECHILD        ,OS_TEXT("No child processes")},
            {EAGAIN        ,OS_TEXT("Try again")},
            {ENOMEM        ,OS_TEXT("Out of memory")},
            {EACCES        ,OS_TEXT("Permission denied")},
            {EFAULT        ,OS_TEXT("Bad address")},
            {ENOTBLK    ,OS_TEXT("Block device required")},
            {EBUSY        ,OS_TEXT("Device or resource busy")},
            {EEXIST        ,OS_TEXT("File exists")},
            {EXDEV        ,OS_TEXT("Cross-device link")},
            {ENODEV        ,OS_TEXT("No such device")},
            {ENOTDIR    ,OS_TEXT("Not a directory")},
            {EISDIR        ,OS_TEXT("Is a directory")},
            {EINVAL        ,OS_TEXT("Invalid argument")},
            {ENFILE        ,OS_TEXT("File table overflow")},
            {EMFILE        ,OS_TEXT("Too many open files")},
            {ENOTTY        ,OS_TEXT("Not a typewriter")},
            {ETXTBSY    ,OS_TEXT("Text file busy")},
            {EFBIG        ,OS_TEXT("File too large")},
            {ENOSPC        ,OS_TEXT("No space left on device")},
            {ESPIPE        ,OS_TEXT("Illegal seek")},
            {EROFS        ,OS_TEXT("Read-only file system")},
            {EMLINK        ,OS_TEXT("Too many links")},
            {EPIPE        ,OS_TEXT("Broken pipe")},
            {EDOM        ,OS_TEXT("Math argument out of domain of func")},
            {ERANGE        ,OS_TEXT("Math result not representable")},

//             //errno.h
//
             {EDEADLK        ,OS_TEXT("Resource deadlock would occur")},
            {ENAMETOOLONG    ,OS_TEXT("File name too long")},
            {ENOLCK            ,OS_TEXT("No record locks available")},
            {ENOSYS            ,OS_TEXT("Function not implemented")},
            {ENOTEMPTY        ,OS_TEXT("Directory not empty")},
            {ELOOP            ,OS_TEXT("Too many symbolic links encountered")},

            {ENOMSG            ,OS_TEXT("No message of desired type")},
            {EIDRM            ,OS_TEXT("Identifier removed")},
            {ECHRNG            ,OS_TEXT("Channel number out of range")},
            {EL2NSYNC        ,OS_TEXT("Level 2 not synchronized")},
            {EL3HLT            ,OS_TEXT("Level 3 halted")},
            {EL3RST            ,OS_TEXT("Level 3 reset")},
            {ELNRNG            ,OS_TEXT("Link number out of range")},
            {EUNATCH        ,OS_TEXT("Protocol driver not attached")},
            {ENOCSI            ,OS_TEXT("No CSI structure available")},
            {EL2HLT            ,OS_TEXT("Level 2 halted")},
            {EBADE            ,OS_TEXT("Invalid exchange")},
            {EBADR            ,OS_TEXT("Invalid request descriptor")},
            {EXFULL            ,OS_TEXT("Exchange full")},
            {ENOANO            ,OS_TEXT("No anode")},
            {EBADRQC        ,OS_TEXT("Invalid request code")},
            {EBADSLT        ,OS_TEXT("Invalid slot")},

            {EBFONT            ,OS_TEXT("Bad font file format")},
            {ENOSTR            ,OS_TEXT("Device not a stream")},
            {ENODATA        ,OS_TEXT("No data available")},
            {ETIME            ,OS_TEXT("Timer expired")},
            {ENOSR            ,OS_TEXT("Out of streams resources")},
            {ENONET            ,OS_TEXT("Machine is not on the network")},
            {ENOPKG            ,OS_TEXT("Package not installed")},
            {EREMOTE        ,OS_TEXT("Object is remote")},
            {ENOLINK        ,OS_TEXT("Link has been severed")},
            {EADV            ,OS_TEXT("Advertise error")},
            {ESRMNT            ,OS_TEXT("Srmount error")},
            {ECOMM            ,OS_TEXT("Communication error on send")},
            {EPROTO            ,OS_TEXT("Protocol error")},
            {EMULTIHOP        ,OS_TEXT("Multihop attempted")},
            {EDOTDOT        ,OS_TEXT("RFS specific error")},
            {EBADMSG        ,OS_TEXT("Not a data message")},
            {EOVERFLOW        ,OS_TEXT("Value too large for defined data type")},
            {ENOTUNIQ        ,OS_TEXT("Name not unique on network")},
            {EBADFD            ,OS_TEXT("File descriptor in bad state")},
            {EREMCHG        ,OS_TEXT("Remote address changed")},
            {ELIBACC        ,OS_TEXT("Can not access a needed shared library")},
            {ELIBBAD        ,OS_TEXT("Accessing a corrupted shared library")},
            {ELIBSCN        ,OS_TEXT(".lib section in a.out corrupted")},
            {ELIBMAX        ,OS_TEXT("Attempting to link in too many shared libraries")},
            {ELIBEXEC        ,OS_TEXT("Cannot exec a shared library directly")},
            {EILSEQ            ,OS_TEXT("Illegal byte sequence")},
            {ERESTART        ,OS_TEXT("Interrupted system call should be restarted")},
            {ESTRPIPE        ,OS_TEXT("Streams pipe error")},
            {EUSERS            ,OS_TEXT("Too many users")},
            {ENOTSOCK        ,OS_TEXT("Socket operation on non-socket")},
            {EDESTADDRREQ    ,OS_TEXT("Destination address required")},
            {EMSGSIZE        ,OS_TEXT("Message too long")},
            {EPROTOTYPE        ,OS_TEXT("Protocol wrong type for socket")},
            {ENOPROTOOPT    ,OS_TEXT("Protocol not available")},
            {EPROTONOSUPPORT,OS_TEXT("Protocol not supported")},
            {ESOCKTNOSUPPORT,OS_TEXT("Socket type not supported")},
            {EOPNOTSUPP        ,OS_TEXT("Operation not supported on transport endpoint")},
            {EPFNOSUPPORT    ,OS_TEXT("Protocol family not supported")},
            {EAFNOSUPPORT    ,OS_TEXT("Address family not supported by protocol")},
            {EADDRINUSE        ,OS_TEXT("Address already in use")},
            {EADDRNOTAVAIL    ,OS_TEXT("Cannot assign requested address")},
            {ENETDOWN        ,OS_TEXT("Network is down")},
            {ENETUNREACH    ,OS_TEXT("Network is unreachable")},
            {ENETRESET        ,OS_TEXT("Network dropped connection because of reset")},
            {ECONNABORTED    ,OS_TEXT("Software caused connection abort")},
            {ECONNRESET        ,OS_TEXT("Connection reset by peer")},
            {ENOBUFS        ,OS_TEXT("No buffer space available")},
            {EISCONN        ,OS_TEXT("Transport endpoint is already connected")},
            {ENOTCONN        ,OS_TEXT("Transport endpoint is not connected")},
            {ESHUTDOWN        ,OS_TEXT("Cannot send after transport endpoint shutdown")},
            {ETOOMANYREFS    ,OS_TEXT("Too many references: cannot splice")},
            {ETIMEDOUT        ,OS_TEXT("Connection timed out")},
            {ECONNREFUSED    ,OS_TEXT("Connection refused")},
            {EHOSTDOWN        ,OS_TEXT("Host is down")},
            {EHOSTUNREACH    ,OS_TEXT("No route to host")},
            {EALREADY        ,OS_TEXT("Operation already in progress")},
            {EINPROGRESS    ,OS_TEXT("Operation now in progress")},
            {ESTALE            ,OS_TEXT("Stale NFS file handle")},
            {EUCLEAN        ,OS_TEXT("Structure needs cleaning")},
            {ENOTNAM        ,OS_TEXT("Not a XENIX named type file")},
            {ENAVAIL        ,OS_TEXT("No XENIX semaphores available")},
            {EISNAM            ,OS_TEXT("Is a named type file")},
            {EREMOTEIO        ,OS_TEXT("Remote I/O error")},
            {EDQUOT            ,OS_TEXT("Quota exceeded")},

            {ENOMEDIUM        ,OS_TEXT("No medium found")},
            {EMEDIUMTYPE    ,OS_TEXT("Wrong medium type")},
            {ECANCELED        ,OS_TEXT("Operation Canceled")},
            {ENOKEY            ,OS_TEXT("Required key not available")},
            {EKEYEXPIRED    ,OS_TEXT("Key has expired")},
            {EKEYREVOKED    ,OS_TEXT("Key has been revoked")},
            {EKEYREJECTED    ,OS_TEXT("Key was rejected by service")},

            {EOWNERDEAD        ,OS_TEXT("Owner died")},
            {ENOTRECOVERABLE,OS_TEXT("State not recoverable")},

            {ERFKILL        ,OS_TEXT("Operation not possible due to RF-kill")},

            {EHWPOISON        ,OS_TEXT("Memory page has hardware error")},
#else
            {10004, OS_TEXT("阻塞操作被函数WSACancelBlockingCall ()调用所中断")},
            {10013, OS_TEXT("试图使用被禁止的访问权限去访问套接字")},
            {10014, OS_TEXT("系统检测到调用试图使用的一个指针参数指向的是一个非法指针地址")},
            {10022, OS_TEXT("提供了非法参数")},
            {10024, OS_TEXT("打开了太多的套接字")},
            {10035, OS_TEXT("当前操作在非阻塞套接字上不能立即完成")},
            {10036, OS_TEXT("一个阻塞操作正在执行")},
            {10037, OS_TEXT("被调用的套接字当前有操作正在进行")},
            {10038, OS_TEXT("操作试图不是在套接字上进行")},
            {10039, OS_TEXT("在套接字上一个操作所必须的地址被遗漏")},
            {10040, OS_TEXT("在数据报套接字上发送的一个消息大于内部消息缓冲区或一些其它网络限制，或者是用来接受数据报的缓冲区小于数据报本身")},
            {10041, OS_TEXT("在socket()函数调用中指定的协议不支持请求的套接字类型的语义")},
            {10042, OS_TEXT("在getsockopt()或setsockopt()调用中，指定了一个未知的、非法的或不支持的选项或层(level)")},
            {10043, OS_TEXT("请求的协议没有在系统中配置或没有支持它的实现存在")},
            {10044, OS_TEXT("不支持在此地址族中指定的套接字类型")},
            {10045, OS_TEXT("对于引用的对象的类型来说，试图进行的操作不支持")},
            {10046, OS_TEXT("协议簇没有在系统中配置或没有支持它的实现存在")},
            {10047, OS_TEXT("使用的地址与被请求的协议不兼容")},
            {10048, OS_TEXT("重复使用一个套接字地址(协议/IP地址/端口号)")},
            {10049, OS_TEXT("被请求的地址在它的环境中是不合法的")},
            {10050, OS_TEXT("套接字操作遇到一个不活动的网络")},
            {10051, OS_TEXT("试图和一个无法到达的网络进行套接字操作")},
            {10052, OS_TEXT("在操作正在进行时连接因“keep-alive”检测到失败而中断")},
            {10053, OS_TEXT("一个已建立的连接被你的主机上的软件终止")},
            {10054, OS_TEXT("存在的连接被远程主机强制关闭")},
            {10055, OS_TEXT("由于系统缺乏足够的缓冲区空间，或因为队列已满")},
            {10056, OS_TEXT("连接请求发生在已经连接的套接字上")},
            {10057, OS_TEXT("因为套接字没有连接，发送或接收数据的请求不被允许，或者是使用sendto()函数在数据报套接字上发送时没有提供地址")},
            {10058, OS_TEXT("因为套接字在相应方向上已经被先前的shutdown()调用关闭，因此该方向上的发送或接收请求不被允许")},
            {10060, OS_TEXT("连接请求因被连接方在一个时间周期内不能正确响应而失败，或已经建立的连接因被连接的主机不能响应而失败")},
            {10061, OS_TEXT("因为目标主机主动拒绝，连接不能建立")},
            {10064, OS_TEXT("套接字操作因为目的主机关闭而失败返回")},
            {10065, OS_TEXT("试图和一个不可达主机进行套接字操作")},
            {10067, OS_TEXT("太多的socket应用程序")},
            {10091, OS_TEXT("此时Windows Sockets实现因底层用来提供网络服务的系统不可用")},
            {10093, OS_TEXT("应用程序没有调用WSAStartup()函数，或函数WSAStartup()调用失败了")},
            {10101, OS_TEXT("远端已经初始化了一个“雅致”的关闭序列")},
            {10109, OS_TEXT("指定的类没有找到")},
            {11001, OS_TEXT("主机未知")},
            {11002, OS_TEXT("在主机名解析时的临时错误，它意味着本地服务器没有从授权服务器接收到一个响应")},
            {11003, OS_TEXT("在数据库查找时发生了某种不可恢复错误")},
            {11004, OS_TEXT("请求的名字合法并且在数据库中找到了，但它没有正确的关联数据用于解析")},
            {11092, OS_TEXT("当前的Windows Sockets实现不支持应用程序指定的Windows Sockets规范版本")},
#endif//HGL_OS != HGL_OS_Windows

            {-1,    OS_TEXT("未知错误")},
        };

        /**
        * 取得SOCKET错误详细信息字符串
        * @param n 错误编号
        * @return 详细信息字符串
        */
        const os_char *GetSocketString(int n)
        {
            int i=0;

            while(tab[i].number!=-1)
            {
                if(tab[i].number==n)
                    return tab[i].recountal;

                i++;
            }

            return(tab[i].recountal);        //此时tab[i].number==-1
        }

        /**
        * @param ThisSocket 要关闭的socket
        */
        void CloseSocket(int ThisSocket)
        {
            if(ThisSocket<=-1)return;

            #ifdef _WIN32
                closesocket(ThisSocket);

                LOG_INFO(OS_TEXT("CloseSocket: ") + OSString(ThisSocket));
            #else
                int result;

                do
                {
                    shutdown(ThisSocket,SHUT_RDWR);     //终止读写操作

                    result=close(ThisSocket);

                    LOG_INFO(OS_TEXT("CloseSocket: ")+OSString(ThisSocket)+OS_TEXT(",result:")+OSString(result)+OS_TEXT(",errno: ")+OSString(errno));

                    if(errno==EBADF)break;
                    if(errno==EINPROGRESS)continue;
                }while(result);

                LOG_INFO(OS_TEXT("CloseSocket: ") + OSString(ThisSocket) + OS_TEXT(",result:") + OSString(result));
            #endif//_WIN32
        }

        /**
        * 设置是否使用堵塞方式传输
        * @param ThisSocket 要设置的socket
        * @param block 是否使用堵塞方式(true/false)
        * @param send_time_out 发送数据超时时间(单位: 秒)
        * @param recv_time_out 接收数据超时时间(单位: 秒)
        */
        void SetSocketBlock(int ThisSocket,bool block,double send_time_out,double recv_time_out)
        {
            #if HGL_OS == HGL_OS_Windows
                u_long par = (block ? 0 : 1);

                DWORD stv=send_time_out*1000;
                DWORD rtv=recv_time_out*1000;

                ioctlsocket(ThisSocket, FIONBIO, &par);

                setsockopt(ThisSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&rtv,sizeof(DWORD));
                setsockopt(ThisSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&stv,sizeof(DWORD));
            #else
                int par=(block?0:1);

                timeval stv,rtv;

                SetTimeVal(stv,send_time_out);
                SetTimeVal(rtv,recv_time_out);

                ioctl(ThisSocket,FIONBIO,&par);

                 setsockopt(ThisSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&rtv,sizeof(timeval));
                 setsockopt(ThisSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&stv,sizeof(timeval));
            #endif//HGL_OS == HGL_OS_Windows
        }

        void SetSocketLinger(int ThisSocket,int time_out)
        {
            struct linger so_linger;

            if(time_out>0)
            {
                so_linger.l_onoff=1;
                so_linger.l_linger=time_out;
            }
            else
            {
                so_linger.l_onoff=0;
            }

            setsockopt(ThisSocket,SOL_SOCKET,SO_LINGER,(const char *)&so_linger,sizeof(linger));
        }
    }//namespace network
}//namespace hgl
