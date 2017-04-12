#include<hgl/algorithm/Hash.h>
#include<hgl/FileSystem.h>
#include<iostream>

using namespace std;
using namespace hgl;
using namespace hgl::algorithm;

#if HGL_OS == HGL_OS_Windows
int wmain(int argc,wchar_t **argv)
#else
int main(int argc,char **argv)
#endif//HGL_OS == HGL_OS_Windows
{
    if(argc<2)
    {
        cout<<"CountHash [filename]"<<endl;
        return(0);
    }

#if HGL_OS == HGL_OS_Windows
    wcout<<L"CountHash \""<<argv[1]<<L'"'<<endl;
#else
    cout<<"CountHash \""<<argv[1]<<<<'"'<<endl;
#endif//HGL_OS == HGL_OS_Windows

    void *file_data;
    int64 file_length;

    file_length=filesystem::LoadFileToMemory(argv[1],&file_data);

    cout<<"file length: "<<file_length<<endl;

    UTF8String hash_name;
    int hash_length;
    char *hash_code;
    char hash_str[(32*2)+1];

    for(int i=hashNone+1;i<hashEnd;i++)
    {
        Hash *h=CreateHash(HASH_ALGORITHML(i));
        hash_length=h->GetHashBytes();

        hash_code=new char[hash_length];

        h->Init();
        h->Update(file_data,file_length);
        h->Final(hash_code);

        ToLowerHexStr(hash_str,hash_code,hash_length);

        hash_str[hash_length*2]=0;

        h->GetName(hash_name);

        cout<<hash_name.c_str()<<":"<<hash_str<<endl;

        delete[] hash_code;
        delete h;
    }
}
