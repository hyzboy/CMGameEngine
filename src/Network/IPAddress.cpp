#include<hgl/network/IP.h>

namespace hgl
{
    namespace network
    {
        bool FillAddr(sockaddr_in &addr,const char *name,int socktype,int protocol)
        {
            if(name)
            {
                struct addrinfo hints, *answer;

                hgl_zero(hints);
                hints.ai_family = AF_INET;
                hints.ai_socktype=socktype;
                hints.ai_protocol=protocol;

                if (getaddrinfo(name, nullptr, &hints, &answer))         //此函数最低Windows 2003/Vista
                    RETURN_FALSE;

                memcpy(&addr,answer->ai_addr,sizeof(sockaddr_in));
                freeaddrinfo(answer);
            }
            else
            {
                hgl_zero(addr);

                addr.sin_family=AF_INET;
            }

            return(true);
        }

        bool FillAddr(sockaddr_in6 &addr,const char *name,int socktype,int protocol)
        {
            if(name)
            {
                struct addrinfo hints, *answer;

                hgl_zero(hints);
                hints.ai_family = AF_INET6;
                hints.ai_socktype=socktype;
                hints.ai_protocol=protocol;

                if (getaddrinfo(name, nullptr, &hints, &answer))         //此函数最低Windows 2003/Vista
                    RETURN_FALSE;

                memcpy(&addr,answer->ai_addr,sizeof(sockaddr_in6));
                freeaddrinfo(answer);
            }
            else
            {
                hgl_zero(addr);

                addr.sin6_family=AF_INET6;
            }

            return(true);
        }

        /**
        * 绑定指定域名/IP和port到当前socket
        * @param ThisSocket 要指定的当前socket
        * @param addr 指定的ip/port
        * @param reuse 是否复用当前地址,默认为1
        * @return 是否成功
        */
        template<typename SockAddr,typename SockAddrIn>
        bool BindAddr(int ThisSocket,const SockAddrIn &addr,int reuse=1)
        {
            const int val = reuse;

#if HGL_OS == HGL_OS_Windows
            setsockopt(ThisSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&val, sizeof(BOOL));     //win下的BOOL本质也是int，所以唯一区分只在于val的传入类型
#else
            setsockopt(ThisSocket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
#endif//HGL_OS == HGL_OS_Windows

            if (bind(ThisSocket, (SockAddr *)&addr, sizeof(SockAddrIn)))
            {
                LOG_ERROR(OS_TEXT("Bind Socket Error! errno: ") + OSString(GetLastSocketError()));
                return(false);
            }

            return(true);
        }

        bool GetHostname(UTF8String &name,const sockaddr *addr)
        {
            char hostname[NI_MAXHOST];
            char server_info[NI_MAXSERV];

            if(getnameinfo(addr,sizeof(struct sockaddr),hostname,NI_MAXHOST,server_info,NI_MAXSERV,NI_NUMERICSERV))
                return(false);

            name=hostname;
            return(true);
        }

        void AddAddrToList(List<in_addr> &addr_list, const sockaddr_in *sai)
        {
            addr_list.Add(sai->sin_addr);
        }

        void AddAddrToList(List<in6_addr> &addr_list, const sockaddr_in6 *sai)
        {
            addr_list.Add(sai->sin6_addr);
        }

        template<int FAMILY,typename InAddr,typename SockAddrIn>
        int GetIPList(List<InAddr> &addr_list,const char *addr_string, int socktype,int protocol)
        {
            struct addrinfo hints, *answer, *ptr;

            hgl_zero(hints);
            hints.ai_family = FAMILY;
            hints.ai_socktype=socktype;
            hints.ai_protocol=protocol;

            if (getaddrinfo(addr_string, nullptr, &hints, &answer))         //此函数最低Windows 2003/Vista
                return(-1);

            int count = 0;
            for (ptr = answer; ptr; ptr = ptr->ai_next)
            {
                AddAddrToList(addr_list,(SockAddrIn *)(ptr->ai_addr));
                ++count;
            }

            freeaddrinfo(answer);
            return(count);
        }

        /**
         * 返回本机的IP支持情况
         * @return 支持的协议数量
         * @return -1 出错，无法获取主机名称
         * @return -2 出错，无法获取地址信息
         */
        int GetIPSupport(List<IPSupport> &ipsl)
        {
            char hostname[NI_MAXHOST];

            if(gethostname(hostname, NI_MAXHOST))
                return(-1);

            struct addrinfo hints, *answer, *ptr;

            hgl_zero(hints);

            if (getaddrinfo(hostname, nullptr, &hints, &answer))         //此函数最低Windows 2003/Vista
                return(-1);

            int count = 0;

            for (ptr = answer; ptr; ptr = ptr->ai_next)
            {
                IPSupport s;

                s.family    =ptr->ai_family;
                s.socktype  =ptr->ai_socktype;
                s.protocol  =ptr->ai_protocol;

                if(ptr->ai_family==AF_INET)
                {
                    memcpy(&(s.ipv4),ptr->ai_addr,ptr->ai_addrlen);

                    inet_ntop(AF_INET,&(s.ipv4.sin_addr),s.ipv4str,INET_ADDRSTRLEN);
                }
                else
                if(ptr->ai_family==AF_INET6)
                {
                    memcpy(&(s.ipv6),ptr->ai_addr,ptr->ai_addrlen);

                    inet_ntop(AF_INET6,&(s.ipv6.sin6_addr),s.ipv6str,INET6_ADDRSTRLEN);
                }

                ipsl.Add(s);

                ++count;
            }

            freeaddrinfo(answer);
            return(count);
        }
    }//namespace network

    namespace network
    {
        bool IPv4Address::Set(const char *name,ushort port,int _socktype,int _protocol)
        {
            socktype=_socktype;
            protocol=_protocol;

            if(!FillAddr(addr,name,socktype,protocol))
                RETURN_FALSE;

            addr.sin_port=htons(port);
            return(true);
        }

        void IPv4Address::Set(ushort port)
        {
            hgl_zero(addr);

            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
        }

        bool IPv4Address::Bind(int ThisSocket,int reuse)const{return BindAddr<sockaddr,sockaddr_in>(ThisSocket,addr,reuse);}
        bool IPv4Address::GetHostname(UTF8String &name)const{return hgl::network::GetHostname(name,(sockaddr *)&addr);}

        const ushort IPv4Address::GetPort()const{return addr.sin_port;}

        void IPv4Address::ToString(char *str)const{inet_ntop(AF_INET,(void *)&(addr.sin_addr),str,INET_ADDRSTRLEN);}

        /**
        * 取得指定域名的IPv4地址列表
        * @param addr_list 存放结果的地址列表
        * @param domain 域名或地址字符串
        * @param _socktype Socket类型(可以为SOCK_STREAM、SOCK_DGRAM、SOCK_RAW、SOCK_RDM、SOCK_SEQPACKET等值),默认为所有类型。
        * @param _protocol 协议类型(可以为IPPROTO_TCP、IPPROTO_UDP、IPPROTO_SCTP),默认为所有类型。
        * @return 地址个数,-1表示出错
        */
        int IPv4Address::GetDomainIPList(List<in_addr> &addr_list,const char *domain,int socktype,int protocol)
        {
            return GetIPList<AF_INET,in_addr,sockaddr_in>(addr_list,domain,socktype,protocol);
        }

        /**
        * 取得本机的IPv4地址列表
        * @param addr_list 存放结果的地址列表
        * @param _socktype Socket类型(可以为SOCK_STREAM、SOCK_DGRAM、SOCK_RAW、SOCK_RDM、SOCK_SEQPACKET等值),默认为所有类型。
        * @param _protocol 协议类型(可以为IPPROTO_TCP、IPPROTO_UDP、IPPROTO_SCTP),默认为所有类型。
        * @return 地址个数,-1表示出错
        */
        int IPv4Address::GetLocalIPList(List<in_addr> &addr_list,int _socktype,int _protocol)
        {
            char hostname[NI_MAXHOST];

            if(gethostname(hostname, NI_MAXHOST))
                return(-1);

            return GetDomainIPList(addr_list,hostname,_socktype,_protocol);
        }

        void IPv4Address::ToString(char str[INET_ADDRSTRLEN],const in_addr &addr)
        {
            inet_ntop(AF_INET,(void *)&addr,str,INET_ADDRSTRLEN);
        }

        bool IPv4Address::Comp(const IPAddress *ipa)const
        {
            if(this==ipa)return(true);
            if(!ipa)return(false);

            if(ipa->GetFamily()!=AF_INET)return(false);
            if(ipa->GetProtocol()!=protocol)return(false);

            return (memcmp(&addr,&(((IPv4Address *)ipa)->addr),sizeof(sockaddr_in))==0);
        }
    }//namespace network

    namespace network
    {
        bool IPv6Address::Set(const char *name,ushort port,int _socktype,int _protocol)
        {
            socktype=_socktype;
            protocol=_protocol;

            if(!FillAddr(addr,name,socktype,protocol))
                RETURN_FALSE;

            addr.sin6_port=htons(port);
            return(true);
        }

        void IPv6Address::Set(ushort port)
        {
            hgl_zero(addr);

            addr.sin6_family = AF_INET6;
            addr.sin6_port = htons(port);
        }

        bool IPv6Address::Bind(int ThisSocket,int reuse)const{return BindAddr<sockaddr,sockaddr_in6>(ThisSocket,addr,reuse);}
        bool IPv6Address::GetHostname(UTF8String &name)const{return hgl::network::GetHostname(name,(sockaddr *)&addr);}

        const ushort IPv6Address::GetPort()const{return addr.sin6_port;}

        void IPv6Address::ToString(char *str)const{inet_ntop(AF_INET6, (void *)&(addr.sin6_addr),str,INET6_ADDRSTRLEN);}

        /**
        * 取得指定域名的IPv6地址列表
        * @param addr_list 存放结果的地址列表
        * @param domain 域名或地址字符串
        * @param _socktype Socket类型(可以为SOCK_STREAM、SOCK_DGRAM、SOCK_RAW、SOCK_RDM、SOCK_SEQPACKET等值),默认为所有类型。
        * @param _protocol 协议类型(可以为IPPROTO_TCP、IPPROTO_UDP、IPPROTO_SCTP),默认为所有类型。
        * @return 地址个数,-1表示出错
        */
        int IPv6Address::GetDomainIPList(List<in6_addr> &addr_list,const char *domain,int socktype,int protocol)
        {
            return GetIPList<AF_INET6,in6_addr,sockaddr_in6>(addr_list,domain,socktype,protocol);
        }

        /**
        * 取得本机的IPv6地址列表
        * @param addr_list 存放结果的地址列表
        * @param _socktype Socket类型(可以为SOCK_STREAM、SOCK_DGRAM、SOCK_RAW、SOCK_RDM、SOCK_SEQPACKET等值),默认为所有类型。
        * @param _protocol 协议类型(可以为IPPROTO_TCP、IPPROTO_UDP、IPPROTO_SCTP),默认为所有类型。
        * @return 地址个数,-1表示出错
        */
        int IPv6Address::GetLocalIPList(List<in6_addr> &addr_list,int _socktype,int _protocol)
        {
            char hostname[NI_MAXHOST];

            if(gethostname(hostname, NI_MAXHOST))
                return(-1);

            return GetDomainIPList(addr_list,hostname,_socktype,_protocol);
        }

        void IPv6Address::ToString(char str[INET6_ADDRSTRLEN],const in6_addr &addr)
        {
            inet_ntop(AF_INET6, (void *)&addr,str,INET6_ADDRSTRLEN);
        }

        bool IPv6Address::Comp(const IPAddress *ipa)const
        {
            if(this==ipa)return(true);
            if(!ipa)return(false);

            if(ipa->GetFamily()!=AF_INET6)return(false);
            if(ipa->GetProtocol()!=protocol)return(false);

            return (memcmp(&addr,&(((IPv6Address *)ipa)->addr),sizeof(sockaddr_in6))==0);
        }
    }//namespace network
}//namespace hgl
