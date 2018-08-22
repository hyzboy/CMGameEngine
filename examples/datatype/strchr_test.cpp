#include<hgl/type/BaseString.h>
#include<iostream>

int main(int,char **)
{
    constexpr char str[]="hello,world!";

    std::cout<<"str: "<<str<<std::endl;

    std::cout<<"strchr(str,',') return "<<hgl::strchr(str,',')<<std::endl;
    std::cout<<"strrchr(str,strlen(str),',') return "<<hgl::strrchr(str,strlen(str),',')<<std::endl;

    std::cout<<"strchr(str,\"el\",2) return "<<hgl::strchr(str,"el",2)<<std::endl;
    std::cout<<"strrchr(str,strlen(str),\"el\",2) return "<<hgl::strrchr(str,strlen(str),"el",2)<<std::endl;

    std::cout<<"strrchr(str,strlen(str),3,'l') return "<<hgl::strrchr(str,strlen(str),3,'l')<<std::endl;
    std::cout<<"strrchr(str,strlen(str),3,\"el\",2) return "<<hgl::strrchr(str,strlen(str),3,"el",2)<<std::endl;

    return 0;
}
