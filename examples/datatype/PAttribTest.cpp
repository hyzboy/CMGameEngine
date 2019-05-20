#include<iostream>
#include<hgl/ut/PAttrib.h>

using namespace hgl;
using namespace std;

int main(int argc,char **argv)
{
    if(argc!=3)
    {
        std::cout<< "PAttribTest use format:\t[cmd] [filename]"<<std::endl<<std::endl<<
                    "Example:"<<std::endl<<
                    "\t\tPAttribTest c 1.txt\t\t\tCreate a plist file."<<std::endl<<
                    "\t\tPAttribTest l 1.txt\t\t\tLoad a plist file."<<std::endl<<std::endl;

        return 0;
    }

    PAttribSet<char> pl_set;

    //                                      属性名称        缺省值         最小值     最大值
    //-------------------------------------+-----------+---------------+--------+----------
    auto *name  =pl_set.CreateStringAttrib(  "name",     "Anonymous");
    auto *sex   =pl_set.CreateBoolAttrib(    "sex",      true);
    auto *age   =pl_set.CreateNumberAttrib(  "age",      17,                 1,      120);

    const char cmd=argv[1][0];

    if(cmd=='c'||cmd=='C')
    {
        pl_set.SaveToTextFile<bomUTF8>(ToOSString(argv[2]));
    }
    else
    {
        pl_set.LoadFromTextFile(ToOSString(argv[2]));

        //lambda方式
        pl_set.Enum([](const BaseString<char> &key,PAttribBase<char> *attr)
        {
            std::cout<<key.c_str()<<":"<<attr->MakeToString().c_str()<<std::endl;
        });

        //传统方式
//         std::cout<<"name:"<<name->MakeToString().c_str()<<std::endl;
//         std::cout<<" sex:"<< sex->MakeToString().c_str()<<std::endl;
//         std::cout<<" age:"<< age->MakeToString().c_str()<<std::endl;
    }

    return(0);
}
