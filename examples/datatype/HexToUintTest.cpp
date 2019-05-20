#include<iostream>
#include<hgl/Str.h>

using namespace hgl;

int main(int,char **)
{
    char str[32];
    uint8 a=0x80;
    uint16 b=0x4567;

    htos(str,32,a   );  std::cout<<"to uint : "<<str<<std::endl;
    htos(str,32,b   );  std::cout<<"to uint : "<<str<<std::endl;
    htos(str,32,str );  std::cout<<"str to uint : "<<str<<std::endl;
    htos(str,32,main);  std::cout<<"main to uint : "<<str<<std::endl;

    return 0;
}
