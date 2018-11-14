#include<hgl/network/MultiThreadAccept.h>
#include<hgl/network/AcceptServer.h>
#include<hgl/Time.h>

namespace hgl
{
    namespace network
    {
        namespace
        {
            void PreMalloc(IPAddressStack &ip_stack,ServerSocket *server)
            {
                int count=ip_stack.GetCount();

                ip_stack.SetMax(count+1024);

                server->CreateIPAddress(ip_stack.GetData()+count,1024);

                ip_stack.SetCount(count+1024);
            }
        }//namespace

        AcceptThread::AcceptThread(AcceptServer *as,Semaphore *sem)
        {
            server=as;
            active_semaphore=sem;

            if(!server)
                return;

            PreMalloc(ip_stack,server);
        }

        bool AcceptThread::Execute()
        {
            if(!server)return(false);

            IPAddress *client_ip;
            int client_sock;

            if(ip_stack.GetCount()<=0)
                PreMalloc(ip_stack,server);

            ip_stack.Pop(client_ip);

            client_sock=server->Accept(client_ip);

            if(client_sock<0)
            {
                ip_stack.Push(client_ip);
                return(false);
            }

            active_semaphore->Post();       //发一个信号证明自己活着

            if(client_sock>0)
            {
                if(!OnAccept(client_sock,client_ip))
                    CloseSocket(client_sock);
            }

            return(true);
        }//bool AcceptThread::Execute()
    }//namespace network
}//namespace hgl
