#pragma once

#include"DataType.h"
    
using MD5Code=char[16];

class MD5
{
    uint32 state[4];
    uint32 count[2];
    uchar buffer[64];

public:

    void Init();

    void Update(const void *input,uint inputLen);

    void Final(MD5Code &digest);
};//class MD5

void GetMD5(MD5Code &,void *buf,int size);
void ToMD5(MD5Code &,const char *);
