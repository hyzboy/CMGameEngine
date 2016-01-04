#include<errno.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#include<stdio.h>
#include<string.h>

void print(const struct sockaddr_in *sai)
{
    char str[INET_ADDRSTRLEN+1];

    inet_ntop(AF_INET,&(sai->sin_addr),str,INET_ADDRSTRLEN);

    printf("IPv4:%s\n",str);
}

void print(const struct sockaddr_in6 *sai)
{
    char str[INET6_ADDRSTRLEN+1];

    inet_ntop(AF_INET6,&(sai->sin6_addr),str,INET6_ADDRSTRLEN);

    printf("IPv6:%s\n",str);
}

void print(const struct addrinfo *ai)
{
    if(ai->ai_socktype==SOCK_STREAM     )printf("SOCK_STREAM ");else
    if(ai->ai_socktype==SOCK_DGRAM      )printf("SOCK_DGRAM ");else
    if(ai->ai_socktype==SOCK_RAW        )printf("SOCK_RAW ");else
    if(ai->ai_socktype==SOCK_RDM        )printf("SOCK_RDM ");else
    if(ai->ai_socktype==SOCK_SEQPACKET  )printf("SOCK_SEQPACKET ");else
        printf("st%d ",ai->ai_socktype);

    if(ai->ai_protocol==IPPROTO_TCP )printf(" TCP ");else
    if(ai->ai_protocol==IPPROTO_UDP )printf(" UDP ");else
    if(ai->ai_protocol==IPPROTO_SCTP)printf(" SCTP ");else
    {
        if(ai->ai_socktype!=SOCK_RAW)                       //raw是原始socket，不会有协议类型
            printf(" protocol_%d ",ai->ai_protocol);        //未知协议
    }

    if(ai->ai_family==AF_INET)print((struct sockaddr_in *)(ai->ai_addr));
    if(ai->ai_family==AF_INET6)print((struct sockaddr_in6 *)(ai->ai_addr));
}

int main(int argc,char **argv)
{
    for(int i=0;i<argc;i++)
        printf("argv[%d]: %s\n",i,argv[i]);

    {
        struct addrinfo hints, *answer, *ptr;

        memset(&hints,0,sizeof(struct addrinfo));

        if (getaddrinfo(argv[1], NULL, &hints, &answer))         //此函数最低Windows 2003/Vista
            return(-1);

        int count = 0;
        for (ptr = answer; ptr; ptr = ptr->ai_next)
        {
            printf("%d:",count);
            print(ptr);

            ++count;
        }
    }

    return 0;
}
