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

    MultiThreadManage<AcceptThread> accept_thread_manage;

    Semaphore sem;

public:

    TestObject()
    {
#if HGL_OS == HGL_OS_Windows
    InitWinSocket();
#endif//

        server_ip=CreateIPv4TCP(10240);

        if(!server.CreateServer(server_ip,1024,false))
            Exit(nullptr,fosExitApp);

        for(int i=0;i<ACCEPT_THREAD_COUNT;i++)
            accept_thread_manage.Add(new AcceptThread(i,&server,&sem));

        accept_thread_manage.Start();
    }

    ~TestObject()
    {
        accept_thread_manage.Close();

        server.CloseServer();
        delete server_ip;
    }

    void Update()
    {
        sem.Acquire();  //没啥用，就是用来等的，用于挂起主线程，空出CPU
    }
};//class TestObject:public ConsoleFlowObject

HGL_CONSOLE_APPLICATION("多接入测试","MultiAccept",new TestObject)
