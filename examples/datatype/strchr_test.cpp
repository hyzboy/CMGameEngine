#include<hgl/type/BaseString.h>
#include<hgl/io/FileSystem.h>
#include<iostream>

void out(const char *source,const char *result)
{
    std::cout<<source<<" return \""<<result<<"\""<<std::endl;
}

void out(const char *source,const hgl::UTF8String &param,const hgl::UTF8String &result)
{
    std::cout<<source<<"(\""<<param.c_str()<<"\") return \""<<result.c_str()<<"\""<<std::endl;
}

int main(int,char **)
{
    constexpr char str[]="hello,world!";

    std::cout<<"str: \""<<str<<"\""<<std::endl;

    out("strchr(str,',')",hgl::strchr(str,','));
    out("strrchr(str,strlen(str),',')",hgl::strrchr(str,strlen(str),','));

    out("strchr(str,\"el\",2)",hgl::strchr(str,"el",2));
    out("strrchr(str,strlen(str),\"el\",2)",hgl::strrchr(str,strlen(str),"el",2));

    out("strrchr(str,strlen(str),3,'l')",hgl::strrchr(str,strlen(str),3,'l'));
    out("strrchr(str,strlen(str),3,\"el\",2)",hgl::strrchr(str,strlen(str),3,"el",2));

    hgl::UTF8String fn("C:\\1.txt");

    out("ClipFilename",fn,hgl::filesystem::ClipFilename(fn));

    hgl::UTF8String tp1("C:\\1\\2");
    hgl::UTF8String tp2("C:\\1\\2\\");
    hgl::UTF8String tp3("C:\\1\\2\\\\");

    out("ClipLastPathname",tp1,hgl::filesystem::ClipLastPathname(tp1));
    out("ClipLastPathname",tp2,hgl::filesystem::ClipLastPathname(tp2));
    out("ClipLastPathname",tp3,hgl::filesystem::ClipLastPathname(tp3));

    return 0;
}
