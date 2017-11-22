#include<hgl/Console.h>
#include<hgl/thread/Thread.h>
#include<hgl/thread/Semaphore.h>
#include<hgl/network/TCPServer.h>
#include<hgl/LogInfo.h>

#include<iostream>

using namespace hgl;

constexpr uint      ACCEPT_THREAD_COUNT =4;     //Accept线程数量

/**
 * 端口监听线程
 */
class AcceptThread:public Thread
{
    int id;
    TCPServer *server;
    Semaphore *sem;

public:

    AcceptThread(int i,TCPServer *s,Semaphore *se)
    {
        id=i;
        server=s;
        sem=se;
    }

    bool Execute() override
    {
        int new_sock;
        IPAddress *client_ip=server->CreateIPAddress();

        new_sock=server->Accept(client_ip);

        if(new_sock<0)
        {
            std::cout<<"["<<id<<"] Accept Error"<<std::endl;
            delete client_ip;
            return(false);
        }

        if(new_sock>0)
        {
            std::cout<<"["<<id<<"] Accept Socket: "<<new_sock<<std::endl;

            CloseSocket(new_sock);      //关掉
            delete client_ip;
        }

        sem->Post();      //发送一个信号
        return(true);
    }
};//class AcceptThread:public Thread

class TestObject:public ConsoleFlowObject
{
    IPAddress *server_ip;
    TCPServer server;

    AcceptThread **     server_accept   =nullptr;   //监听线程

    Semaphore sem;

public:

    TestObject()
    {
        server_ip=CreateIPv4TCP(10240);

        if(!server.CreateServer(server_ip,1024,false))
            Exit(nullptr,fosExitApp);

        server_accept=new AcceptThread *[ACCEPT_THREAD_COUNT];

        for(int i=0;i<ACCEPT_THREAD_COUNT;i++)
        {
            server_accept[i]=new AcceptThread(i,&server,&sem);

            server_accept[i]->Start();
        }
    }

    ~TestObject()
    {
        for(int i=0;i<ACCEPT_THREAD_COUNT;i++)
        {
            server_accept[i]->Thread::Close();

            delete server_accept[i];
        }

        delete[] server_accept;

        server.CloseServer();
        delete server_ip;
    }

    void Update()
    {
        sem.Acquire();  //没啥用，就是用来等的，用于挂起主线程，空出CPU
    }
};//class TestObject:public ConsoleFlowObject

HGL_CONSOLE_APPLICATION("多接入测试","MultiAccept",new TestObject)
