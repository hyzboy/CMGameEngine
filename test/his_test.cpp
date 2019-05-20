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

    cout<<endl<<endl;
}

int main(int,char **)
{
    HTTPInputStream his;

    const UTF8String host=U8_TEXT("www.hyzgame.com.cn");

    SharedPtr<IPAddress> ip=CreateIPv4TCP(host,80);

    if(!his.Open(ip,host,U8_TEXT("/?page_id=203")))
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

        if(!response)
        {
            uint code=his.GetResponseCode();

            if(code)
            {
                response=true;
                out_response(&his);

                if(his.GetResponseCode()!=200)
                    break;
            }
        }

        if(size>0)
        {
            buf[size]=0;
            cout<<buf;
        }
    }while(size>=0);

    return 0;
}
