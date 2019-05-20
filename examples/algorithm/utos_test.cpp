#include<iostream>
#include<stdlib.h>
#include<hgl/Str.h>

using namespace std;
using namespace hgl;

int main(int,char **)
{
    srand48(time(nullptr));

    long int rr=lrand48();
    char str[1024];

    for(uint i=2;i<=26+10;i++)
    {
        utos(str,1024,rr,i);
        cout<<i<<":"<<str<<endl;
    }

    return 0;
}
