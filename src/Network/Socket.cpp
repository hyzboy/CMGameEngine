#include<hgl/network/Socket.h>
#include<hgl/LogInfo.h>
#include<hgl/Other.h>
#include<time.h>
#include<iostream>

#if HGL_OS != HGL_OS_Windows
#include<netinet/tcp.h>

	#if HGL_OS == HGL_OS_Solaris
		#include<sys/filio.h>
	#endif//HGL_OS == HGL_OS_Solaris
#else//HGL_OS != HGL_OS_Windows
namespace
{
	class WindowsSocketInit
	{
	public:

		static bool ws_init;

	public:

		WindowsSocketInit()
		{
			WSADATA wsa;

			ws_init=(WSAStartup(MAKEWORD(2,2),&wsa)==NO_ERROR);
		}
	};//class WindowsSocketInit

	bool WindowsSocketInit::ws_init=false;
	static WindowsSocketInit wsi;
}//HGL_OS != HGL_OS_Windows
#endif//HGL_OS != HGL_OS_Windows

//setsockopt函数的Windows下使用参数格式请参见http://msdn.microsoft.com/en-us/library/windows/desktop/ms740476(v=vs.85).aspx

namespace hgl
{
	void SetTimeVal(timeval &tv,const double t_sec);

	namespace network
	{
//		static atom_int socket_count=0;

		Socket::Socket()
		{
			ThisSocket=-1;

			socket_domain=-1;
			socket_type=-1;
			socket_protocols=-1;

//			LOG_INFO(u8"Socket Count ++: "+UTF8String(++socket_count));
		}

		Socket::~Socket()
		{
			CloseSocket();

//			LOG_INFO(u8"Socket Count --: "+UTF8String(--socket_count));
		}

		bool Socket::CreateSocket(int d,int t,int p)
		{
			#if HGL_OS == HGL_OS_Windows
			if(!WindowsSocketInit::ws_init)
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
// 			//errno-base.h
// 			#define	EPERM		 1	/* Operation not permitted */
// 			#define	ENOENT		 2	/* No such file or directory */
// 			#define	ESRCH		 3	/* No such process */
			{EINTR,		OS_TEXT("Interrupted system call")},
			{EIO,		OS_TEXT("I/O error")},
// 			#define	ENXIO		 6	/* No such device or address */
// 			#define	E2BIG		 7	/* Argument list too long */
// 			#define	ENOEXEC		 8	/* Exec format error */
// 			#define	EBADF		 9	/* Bad file number */
// 			#define	ECHILD		10	/* No child processes */
			{EAGAIN,	OS_TEXT("Try again")},
			{ENOMEM,	OS_TEXT("Out of memory")},
			{EACCES,	OS_TEXT("Permission denied")},
			{EFAULT,	OS_TEXT("Bad address")},
// 			#define	ENOTBLK		15	/* Block device required */
			{EBUSY,		OS_TEXT("Device or resource busy")},
// 			#define	EEXIST		17	/* File exists */
// 			#define	EXDEV		18	/* Cross-device link */
// 			#define	ENODEV		19	/* No such device */
// 			#define	ENOTDIR		20	/* Not a directory */
// 			#define	EISDIR		21	/* Is a directory */
			{EINVAL,	OS_TEXT("Invalid argument")},
// 			#define	ENFILE		23	/* File table overflow */
			{EMFILE,	OS_TEXT("Too many open files")},
// 			#define	ENOTTY		25	/* Not a typewriter */
// 			#define	ETXTBSY		26	/* Text file busy */
// 			#define	EFBIG		27	/* File too large */
// 			#define	ENOSPC		28	/* No space left on device */
// 			#define	ESPIPE		29	/* Illegal seek */
// 			#define	EROFS		30	/* Read-only file system */
			{EMLINK,	OS_TEXT("Too many links")},
			{EPIPE,		OS_TEXT("Broken pipe")},
// 			#define	EDOM		33	/* Math argument out of domain of func */
// 			#define	ERANGE		34	/* Math result not representable */
//
// 			//errno.h
//
// 			#define	EDEADLK		35	/* Resource deadlock would occur */
// 			#define	ENAMETOOLONG	36	/* File name too long */
// 			#define	ENOLCK		37	/* No record locks available */
// 			#define	ENOSYS		38	/* Function not implemented */
// 			#define	ENOTEMPTY	39	/* Directory not empty */
// 			#define	ELOOP		40	/* Too many symbolic links encountered */
// 			#define	EWOULDBLOCK	EAGAIN	/* Operation would block */
// 			#define	ENOMSG		42	/* No message of desired type */
// 			#define	EIDRM		43	/* Identifier removed */
// 			#define	ECHRNG		44	/* Channel number out of range */
// 			#define	EL2NSYNC	45	/* Level 2 not synchronized */
// 			#define	EL3HLT		46	/* Level 3 halted */
// 			#define	EL3RST		47	/* Level 3 reset */
// 			#define	ELNRNG		48	/* Link number out of range */
// 			#define	EUNATCH		49	/* Protocol driver not attached */
// 			#define	ENOCSI		50	/* No CSI structure available */
// 			#define	EL2HLT		51	/* Level 2 halted */
// 			#define	EBADE		52	/* Invalid exchange */
// 			#define	EBADR		53	/* Invalid request descriptor */
// 			#define	EXFULL		54	/* Exchange full */
// 			#define	ENOANO		55	/* No anode */
// 			#define	EBADRQC		56	/* Invalid request code */
// 			#define	EBADSLT		57	/* Invalid slot */
//
// 			#define	EDEADLOCK	EDEADLK
//
// 			#define	EBFONT		59	/* Bad font file format */
// 			#define	ENOSTR		60	/* Device not a stream */
// 			#define	ENODATA		61	/* No data available */
// 			#define	ETIME		62	/* Timer expired */
// 			#define	ENOSR		63	/* Out of streams resources */
// 			#define	ENONET		64	/* Machine is not on the network */
// 			#define	ENOPKG		65	/* Package not installed */
// 			#define	EREMOTE		66	/* Object is remote */
// 			#define	ENOLINK		67	/* Link has been severed */
// 			#define	EADV		68	/* Advertise error */
// 			#define	ESRMNT		69	/* Srmount error */
// 			#define	ECOMM		70	/* Communication error on send */
// 			#define	EPROTO		71	/* Protocol error */
// 			#define	EMULTIHOP	72	/* Multihop attempted */
// 			#define	EDOTDOT		73	/* RFS specific error */
// 			#define	EBADMSG		74	/* Not a data message */
// 			#define	EOVERFLOW	75	/* Value too large for defined data type */
// 			#define	ENOTUNIQ	76	/* Name not unique on network */
// 			#define	EBADFD		77	/* File descriptor in bad state */
// 			#define	EREMCHG		78	/* Remote address changed */
// 			#define	ELIBACC		79	/* Can not access a needed shared library */
// 			#define	ELIBBAD		80	/* Accessing a corrupted shared library */
// 			#define	ELIBSCN		81	/* .lib section in a.out corrupted */
// 			#define	ELIBMAX		82	/* Attempting to link in too many shared libraries */
// 			#define	ELIBEXEC	83	/* Cannot exec a shared library directly */
// 			#define	EILSEQ		84	/* Illegal byte sequence */
// 			#define	ERESTART	85	/* Interrupted system call should be restarted */
// 			#define	ESTRPIPE	86	/* Streams pipe error */
// 			#define	EUSERS		87	/* Too many users */
			{ENOTSOCK,		OS_TEXT("Socket operation on non-socket")},
// 			#define	EDESTADDRREQ	89	/* Destination address required */
// 			#define	EMSGSIZE	90	/* Message too long */
// 			#define	EPROTOTYPE	91	/* Protocol wrong type for socket */
// 			#define	ENOPROTOOPT	92	/* Protocol not available */
// 			#define	EPROTONOSUPPORT	93	/* Protocol not supported */
// 			#define	ESOCKTNOSUPPORT	94	/* Socket type not supported */
// 			#define	EOPNOTSUPP	95	/* Operation not supported on transport endpoint */
// 			#define	EPFNOSUPPORT	96	/* Protocol family not supported */
// 			#define	EAFNOSUPPORT	97	/* Address family not supported by protocol */
			{EADDRINUSE,	OS_TEXT("Address already in use")},
			{EADDRNOTAVAIL,	OS_TEXT("Cannot assign requested address")},
// 			#define	ENETDOWN	100	/* Network is down */
// 			#define	ENETUNREACH	101	/* Network is unreachable */
// 			#define	ENETRESET	102	/* Network dropped connection because of reset */
// 			#define	ECONNABORTED	103	/* Software caused connection abort */
			{ECONNRESET,	OS_TEXT("Connection reset by peer")},
			{ENOBUFS,		OS_TEXT("No buffer space available")},
// 			#define	EISCONN		106	/* Transport endpoint is already connected */
// 			#define	ENOTCONN	107	/* Transport endpoint is not connected */
// 			#define	ESHUTDOWN	108	/* Cannot send after transport endpoint shutdown */
// 			#define	ETOOMANYREFS	109	/* Too many references: cannot splice */
			{ETIMEDOUT,		OS_TEXT("Connection timed out")},
			{ECONNREFUSED,	OS_TEXT("Connection refused")},
// 			#define	EHOSTDOWN	112	/* Host is down */
// 			#define	EHOSTUNREACH	113	/* No route to host */
// 			#define	EALREADY	114	/* Operation already in progress */
// 			#define	EINPROGRESS	115	/* Operation now in progress */
// 			#define	ESTALE		116	/* Stale NFS file handle */
// 			#define	EUCLEAN		117	/* Structure needs cleaning */
// 			#define	ENOTNAM		118	/* Not a XENIX named type file */
// 			#define	ENAVAIL		119	/* No XENIX semaphores available */
// 			#define	EISNAM		120	/* Is a named type file */
			{EREMOTEIO,		OS_TEXT("Remote I/O error")},
// 			#define	EDQUOT		122	/* Quota exceeded */
//
// 			#define	ENOMEDIUM	123	/* No medium found */
// 			#define	EMEDIUMTYPE	124	/* Wrong medium type */
// 			#define	ECANCELED	125	/* Operation Canceled */
			{ECANCELED,		OS_TEXT("Operation Canceled")},
// 			#define	ENOKEY		126	/* Required key not available */
// 			#define	EKEYEXPIRED	127	/* Key has expired */
// 			#define	EKEYREVOKED	128	/* Key has been revoked */
// 			#define	EKEYREJECTED	129	/* Key was rejected by service */
//
// 			/* for robust mutexes */
// 			#define	EOWNERDEAD	130	/* Owner died */
// 			#define	ENOTRECOVERABLE	131	/* State not recoverable */
//
// 			#define ERFKILL		132	/* Operation not possible due to RF-kill */
//
// 			#define EHWPOISON	133	/* Memory page has hardware error */
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

			return(tab[i].recountal);		//此时tab[i].number==-1
		}

		/**
		* 取得本机IP地址
		* @param address 本机IP地址存放指针(如果取得成功，需要手动delete[]掉)
		* @param hostname 本机hostname,必须不得少于256字节
		* @return 本机IP地址数据,-1表示失败
		*/
		int GetLocalIP(in_addr **address,char *hostname)
		{
			if (gethostname(hostname, 256) !=0)
				return(-1);

			hostent *phe = gethostbyname(hostname);

			int count=0;

			for (int i = 0; phe->h_addr_list[i] != 0; ++i)
				count++;

			*address=new in_addr[count];

			for(int i=0;i<count;i++)
				memcpy(&((*address)[i]), phe->h_addr_list[i], sizeof(in_addr));

			return(count);
		}

		/**
		* 将一个域名转换成IP(IPv4格式)
		* @param name 域名
		* @param address 本机IP地址存放指针(如果取得成功，需要手动delete[]掉)
		* @return 本机IP地址数据,-1表示失败
		*/
		int Domain2IP(const UTF8String &name,in_addr **address)
		{
			if(name.Length()<=0)return(-1);

			int result=-1;
			in_addr iaHost;
			hostent *HostEntry;

			memset(&iaHost,0,sizeof(in_addr));

			iaHost.s_addr=inet_addr(name);

			if(iaHost.s_addr==INADDR_NONE)HostEntry=gethostbyname(name);
									 else HostEntry=gethostbyaddr((char *)&iaHost,sizeof(in_addr),AF_INET);

			if(HostEntry)
			{
				int count=0;

				for (int i = 0; HostEntry->h_addr_list[i] != 0; ++i)
					count++;

				*address=new in_addr[count];

				for(int i=0;i<count;i++)
					memcpy(&((*address)[i]), HostEntry->h_addr_list[i], sizeof(in_addr));

				result=count;
			}
			else
			{
				*address=new in_addr[1];

				*((uint32 *)(*address))=inet_addr(name);

				result=1;
			}

			return(result);
		}

		/**
		* 将指定域名/IPv4地址和port填充到SockAddr结构中
		* @param addr netaddr结构
		* @param name 域名或IPv4地址
		* @param port 端口
		* @return 是否成功
		*/
		bool FillAddr(sockaddr_in *addr,const UTF8String &name,int port)
		{
			if(!addr)return(false);

			if(name.Length()<=0
			 ||name=="0.0.0.0")
			{
				addr->sin_family     =AF_INET;
				addr->sin_addr.s_addr=INADDR_ANY;
				addr->sin_port       =htons(port);
			}
			else
			{
				in_addr_t saddr=inet_addr(name);

#if HGL_OS == HGL_OS_Windows
				hostent *HostEntry;

				if(saddr==INADDR_NONE)HostEntry=gethostbyname(name);
								 else HostEntry=gethostbyaddr((char *)&saddr,sizeof(in_addr_t),AF_INET);

				if(HostEntry)addr->sin_addr.s_addr=((in_addr *)*HostEntry->h_addr_list)->s_addr;
						else addr->sin_addr.s_addr=saddr;
#else
				//手册讲gethostbyaddr_r和gethostbyname_r线程安全，但valgrind-helgrind会报错
				hostent hostbuf;
				hostent *res=nullptr;
				char buf[8192];
				int err=0;

				if(saddr==INADDR_NONE)
				{
					gethostbyname_r(name,
									&hostbuf,buf,sizeof(buf),
									&res,&err);
				}
				else
				{
					gethostbyaddr_r((char *)&saddr,sizeof(in_addr_t),AF_INET,
									&hostbuf,buf,sizeof(buf),
									&res,&err);
				}

				if(res)
					addr->sin_addr.s_addr=((in_addr *)*(hostbuf.h_addr_list))->s_addr;
				else
					addr->sin_addr.s_addr=saddr;
#endif//HGL_OS == HGL_OS_Windows

				addr->sin_family    =AF_INET;
				addr->sin_port      =htons(port);
			}

			return(true);
		}

		/**
		* 绑定指定域名/IP和port到当前socket
		* @param addr 指定的ip/port
		* @return 是否成功
		*/
		bool BindAddr(int ThisSocket,const sockaddr_in &addr)
		{
#if HGL_OS == HGL_OS_Windows
			const BOOL val=true;

			setsockopt(ThisSocket,SOL_SOCKET,SO_REUSEADDR,(const char *)&val,sizeof(BOOL));
#else
			const int val=1;
			setsockopt(ThisSocket,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(int));
#endif//HGL_OS == HGL_OS_Windows

			if(bind(ThisSocket,(sockaddr *)&addr,sizeof(sockaddr_in)))
			{
				LOG_ERROR(OS_TEXT("Bind Socket Error! errno: ")+OSString(GetLastSocketError()));
				std::cerr<<"Bind Socket Error! errno: "<<GetLastSocketError()<<std::endl;
				return(false);
			}

			return(true);
		}

		/**
		* 绑定指定域名/IP和port到当前socket
		* @param name 域名或IP
		* @param port 端口
		* @return 是否成功
		*/
		bool BindAddr(int ThisSocket,const char *name,int port)
		{
			sockaddr_in addr;

			if(!FillAddr(&addr,name,port))return(false);

			return BindAddr(ThisSocket,addr);
		}

		/**
		* @param ThisSocket 要关闭的socket
		*/
		void CloseSocket(int ThisSocket)
		{
			if(ThisSocket<=-1)return;

			#ifdef _WIN32
				closesocket(ThisSocket);
			#else
				close(ThisSocket);
			#endif//_WIN32

			LOG_INFO(OS_TEXT("CloseSocket: ")+OSString(ThisSocket));
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
				so_linger.l_onoff=true;
				so_linger.l_linger=time_out;
			}
			else
			{
				so_linger.l_onoff=false;
			}

			setsockopt(ThisSocket,SOL_SOCKET,SO_LINGER,&so_linger,sizeof(linger));
		}
	}//namespace network
}//namespace hgl
