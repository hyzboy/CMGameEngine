#include<hgl/Console.h>
#include<hgl/network/HTTPInputStream.h>
#include<iostream>

using namespace hgl;
using namespace hgl::network;

using namespace std;

void out_response(HTTPInputStream *his)
{
    const int64 file_length=his->GetSize();

    cout<<"FileLength: "<<file_length<<endl;
    cout<<"HTTP Response: "<<his->GetResponseInfo().c_str()<<endl;

    const UTF8PList &rl=his->GetResponseList();

    UTF8String key,value;

    for(int i=0;i<rl.GetCount();i++)
    {
        rl.Get(i,key,value);

        cout<<key.c_str()<<": "<<value.c_str()<<endl;
    }
}

int main(int,char **)
{
    HTTPInputStream his;

    const UTF8String host=U8_TEXT("cmake.org");

    SharedPtr<IPAddress> ip=CreateIPv4TCP(host,80);

    if(!his.Open(ip,host,U8_TEXT("/cmake/help/v3.3/command/function.html")))
    {
        cerr<<"http open error"<<endl;
        return 0;
    }

    bool response=false;
    int size;
    char buf[1024];

    do
    {
        size=his.Read(buf,1023);

        if(size>0)
        {
            buf[size]=0;
            cout<<buf;
        }

        if(!response&&his.GetResponseCode()!=0)
        {
            response=true;

            out_response(&his);

            if(his.GetResponseCode()!=200)
                break;
        }
    }while(size>=0);

    return 0;
}
