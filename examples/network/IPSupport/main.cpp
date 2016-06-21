#include<hgl/network/IP.h>
#include<hgl/type/List.h>
#include<iostream>

using namespace hgl;
using namespace hgl::network;
using namespace std;

int main(int,char **)
{
    cout<<"Local IP Support"<<endl;

    List<IPSupport> ipsl;

    const int count=GetIPSupport(ipsl);

    for(int i=0;i<count;i++)
    {
        IPSupport s=ipsl[i];

        cout<<"Famile: ";
        switch(s.family)
        {
            case AF_INET:   cout<<"IPv4";break;
            case AF_INET6:  cout<<"IPv6";break;

#if HGL_OS == HGL_OS_Windows

            case AF_NETBIOS: cout<<"NetBIOS";break;
#endif//Windows

            default:        cout<<"Other("<<s.family<<")";break;
        }

        cout<<" SocketType: ";
        switch(s.socktype)
        {
            case SOCK_STREAM:   cout<<"STREAM";break;
            case SOCK_DGRAM:    cout<<"DGRAM";break;
            case SOCK_RAW:      cout<<"RAW";break;
            case SOCK_RDM:      cout<<"RDM";break;
            case SOCK_SEQPACKET:cout<<"SEQPACKET";break;
            case SOCK_DCCP:     cout<<"DCCP";break;
            case SOCK_PACKET:   cout<<"PACKET";break;

            default:            cout<<"Other("<<s.socktype<<")";break;
        }

        cout<<" Protocol: ";
        switch(s.protocol)
        {
            case IPPROTO_TCP:   cout<<"TCP";break;
            case IPPROTO_UDP:   cout<<"UDP";break;
            case IPPROTO_SCTP:  cout<<"SCTP";break;
            default:            cout<<"Other("<<s.protocol<<")";break;
        }

        cout<<endl;
    }

    return 0;
}
