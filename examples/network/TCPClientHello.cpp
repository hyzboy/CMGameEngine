#include<hgl/Console.h>
#include<hgl/network/TCPClient.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/io/DataInputStream.h>

#include<iostream>

using namespace hgl;
using namespace hgl::network;

using namespace std;

HGL_CONSOLE_MAIN(sii,app,args)
{
    sii.info.ProjectName=U8_TEXT("接入测试客户端");
    sii.info.ProjectCode=OS_TEXT("TCPClientHello");

    if(!app.Init(&sii))
        return(-1);

    IPAddress *ip=CreateIPv4TCP("127.0.0.1",10240);

    TCPClient client;

    if(!client.Connect(ip))
    {
        cout<<"connect error"<<endl;
        return(-2);
    }

    cout<<"connect ok"<<endl;

    //发送
    {
        const UTF8String str="Hello,World!";

        io::LEDataOutputStream dos(client.GetOutputStream());
        if(dos.WriteUTF8String(str))
            cout<<"send ok,str: "<<str.c_str()<<endl;
    }

    int result=client.WaitRecv(60);

    if(result>0)//接收
    {
        UTF8String str;

        io::LEDataInputStream dis(client.GetInputStream());

        if(dis.ReadUTF8String(str))
            cout<<"recv ok,str: "<<str.c_str()<<endl;
    }

    client.Disconnect();
    return 0;
}

