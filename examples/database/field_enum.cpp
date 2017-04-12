#include<hgl/db/cdb/FieldSingle.h>
#include<hgl/db/cdb/FieldArray.h>
#include<iostream>

using namespace hgl;
using namespace hgl::cdb;

int main(int,char **)
{
    FieldSingle *fs;
    FieldArray *fa;

    for(int ft=ftBool;ft<ftCombo;ft++)
    {
        fs=CreateFieldSingle((FieldType)ft);

        std::cout<<"Field["<<fs->GetName()<<"] single length="<<fs->GetBytes(1)
                                                     <<",x12="<<fs->GetBytes(12)
                                                     <<",x16="<<fs->GetBytes(16)
                                                     <<std::endl;

        fa=CreateFieldArray(fs,8);

        std::cout<<"FieldArray["<<fs->GetName()<<","<<fa->GetMaxCount()<<"] length="<<fa->GetBytes(1)<<" max="<<fa->GetMaxBytes()<<std::endl;

        delete fs;
    }

    return(0);
}
