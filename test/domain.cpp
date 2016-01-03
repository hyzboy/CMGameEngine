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
    uint8_t ip[4];

    memcpy(ip,&(sai->sin_addr.s_addr),4);

    printf(" %d.%d.%d.%d:%d\n",ip[0],ip[1],ip[2],ip[3],sai->sin_port);
}

void print(const struct addrinfo *ai)
{
    if(ai->ai_socktype==SOCK_STREAM     )printf("SOCK_STREAM");else
    if(ai->ai_socktype==SOCK_DGRAM      )printf("SOCK_DGRAM");else
    if(ai->ai_socktype==SOCK_RAW        )printf("SOCK_RAW");else
    if(ai->ai_socktype==SOCK_RDM        )printf("SOCK_RDM");else
    if(ai->ai_socktype==SOCK_SEQPACKET  )printf("SOCK_SEQPACKET");else
        printf(" st%d",ai->ai_socktype);

    if(ai->ai_protocol==IPPROTO_TCP )printf(" TCP ");else
    if(ai->ai_protocol==IPPROTO_UDP )printf(" UDP ");else
    if(ai->ai_protocol==IPPROTO_SCTP)printf(" SCTP ");else
    {
        if(ai->ai_socktype!=SOCK_RAW)                       //raw是原始socket，不会有协议类型
            printf(" protocol_%d ",ai->ai_protocol);        //未知协议
    }

    print((struct sockaddr_in *)(ai->ai_addr));
}

int main(int argc,char **argv)
{
    for(int i=0;i<argc;i++)
        printf("argv[%d]: %s\n",i,argv[i]);

    {
        struct addrinfo hints, *answer, *ptr;

        memset(&hints,0,sizeof(struct addrinfo));
        hints.ai_family = AF_INET;

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
