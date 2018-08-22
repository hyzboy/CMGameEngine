#include<hgl/type/BaseString.h>
#include<hgl/FileSystem.h>
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

    hgl::UTF8String fn("C:\\1.txt");

    std::cout<<"ClipFilename(\""<<fn.c_str()<<"\") return "<<hgl::filesystem::ClipFilename(fn)<<std::endl;

    hgl::UTF8String tp1("C:\\1\\2");
    hgl::UTF8String tp2("C:\\1\\2\\");
    hgl::UTF8String tp3("C:\\1\\2\\\\");

    std::cout<<"ClipLastPathname(\""<<tp1.c_str()<<"\") return "<<hgl::filesystem::ClipLastPathname(tp1).c_str()<<std::endl;
    std::cout<<"ClipLastPathname(\""<<tp2.c_str()<<"\") return "<<hgl::filesystem::ClipLastPathname(tp2).c_str()<<std::endl;
    std::cout<<"ClipLastPathname(\""<<tp3.c_str()<<"\") return "<<hgl::filesystem::ClipLastPathname(tp3).c_str()<<std::endl;

    return 0;
}
