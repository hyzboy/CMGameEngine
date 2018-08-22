#include<hgl/EnumFile.h>
#include<iostream>

using namespace hgl;
using namespace hgl::filesystem;

#if HGL_OS==HGL_OS_Windows
    constexpr wchar_t example_path[]="C:\\Windows";

    #define std_cout    std::wcout
    #define main_func   wmain
#else
    constexpr char example_path[]="/home/users";

    #define std_cout    std::cout
    #define main_func   main
#endif//

class MyEnumFile:public EnumFile
{
public:

    void ProcFolder(struct EnumFileConfig *parent_efc,struct EnumFileConfig *cur_efc,FileInfo &fi) override
    {
        std_cout<<OS_TEXT("[Folder] ")<<fi.fullname<<std::endl;
    }

    void ProcFile(struct EnumFileConfig *,FileInfo &fi) override
    {
        std_cout<<OS_TEXT("[File] ")<<fi.fullname<<std::endl;
    }
};//class EnumFile

int main_func(int argc,os_char **argv)
{
    std_cout<<OS_TEXT("EnumFile test")<<std::endl<<std::endl;

    if(argc!=2)
    {
        std_cout<<OS_TEXT("example: EnumFileTest ")<<example_path<<std::endl;
        return(0);
    }

    std_cout<<OS_TEXT("enum folder: ")<<argv[1]<<std::endl;

    EnumFileConfig efc(argv[1]);
    MyEnumFile ef;

    efc.sub_folder=true;

    ef.Enum(&efc);
    return 0;
}
