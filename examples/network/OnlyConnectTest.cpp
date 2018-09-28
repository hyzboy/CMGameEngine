#include<hgl/Console.h>
#include<hgl/thread/Thread.h>
#include<hgl/thread/Semaphore.h>
#include<hgl/network/TCPClient.h>

#include<iostream>

using namespace hgl;

constexpr uint      CONNECT_THREAD_COUNT    =4;
constexpr double    CONNECT_TIME_GAP        =0.25;
constexpr double    CONNECT_TIME_GAP_ERROR  =3;

class ConnectThread:public Thread
{
    IPAddress *server_ip;
    Semaphore *sem;

public:

    ConnectThread(IPAddress *ip,Semaphore *s)
    {
        server_ip=ip;
        sem=s;
    }

    bool Execute() override
    {
        TCPClient *tcp=CreateTCPClient(server_ip);

        double wait_time;

        if(tcp)
        {
            delete tcp;
            std::cout<<"Connect to server OK!"<<std::endl;

            wait_time=CONNECT_TIME_GAP;
        }
        else
        {
            const int err=GetLastSocketError();

            std::cout<<"Connect to server failed,return errno: "<<err<<std::endl;

            wait_time=CONNECT_TIME_GAP_ERROR;
        }

        sem->Post();
        Thread::WaitExit(wait_time);
        return(true);
    }
};//class ConnectThread:public Thread

class TestObject:public ConsoleFlowObject
{
    IPAddress *server_ip;

    Semaphore sem;

    MultiThreadManage<Thread> connect_thread_manage;

public:

    TestObject()
    {
        server_ip=CreateIPv4TCP("127.0.0.1",10240);

        for(uint i=0;i<CONNECT_THREAD_COUNT;i++)
            connect_thread_manage.Add(new ConnectThread(server_ip,&sem));

        connect_thread_manage.Start();
    }

    ~TestObject()
    {
        connect_thread_manage.Close();
        delete server_ip;
    }

    void Update()
    {
        sem.Acquire();  //没啥用，就是用来等的，用于挂起主线程，空出CPU
    }
};//class TestObject:public ConsoleFlowObject

HGL_CONSOLE_APPLICATION("纯连接测试","OnlyConnect",new TestObject)
