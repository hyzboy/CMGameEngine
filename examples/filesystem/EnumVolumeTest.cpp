#include<hgl/filesystem/EnumVolume.h>
#include<iostream>

using namespace std;
using namespace hgl;
using namespace hgl::filesystem;

constexpr wchar_t *DriverTypeName[]={
    L"",
    L"Removable",
    L"Fixed",
    L"Remote",
    L"CDROM",
    L"RAM Disk"};

int main(int,char **)
{
#if HGL_OS != HGL_OS_Windows
    cerr<<"this example only support Microsoft Windows platform."<<endl;
#else
    VolumeCheckConfig vcc;                          //卷宗枚举配置

    vcc.SetFullCheck();

    List<VolumeInfo> vi_list;

    int rv=EnumVolume(vi_list,vcc);

    if(rv<0)
    {
        cerr<<"EnumVolume return error: "<<rv<<endl;
    }
    else
    {
        vi_list.Enum([](VolumeInfo &vi)
        {
            wcout<<L"        Volume name: "<<vi.name<<endl;
            wcout<<L"               path: "<<vi.path<<endl;
            wcout<<L"               type: "<<DriverTypeName[vi.driver_type]<<endl;
            wcout<<L"             serial: "<<vi.serial<<endl;
            wcout<<L"              label: "<<vi.volume_label<<endl;
            wcout<<L"        file system: "<<vi.file_system<<endl;
            wcout<<L"filename max length: "<<vi.filename_max_length<<endl;
            wcout<<L"    support unicode: "<<(vi.unicode?L"YES":L"NO")<<endl;
            wcout<<L"    available space: "<<vi.available_space<<endl;
            wcout<<L"        total space: "<<vi.total_space<<endl;
            wcout<<L"         free space: "<<vi.free_space<<endl;
            wcout<<endl;
        });
    }
#endif//
    return 0;
}
