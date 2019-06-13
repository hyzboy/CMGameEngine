#pragma once

#include<string>

using  int8     =  signed char;         ///<有符号 8位整型
using uint8     =unsigned char;         ///<无符号 8位整型
using  int16    =  signed short;        ///<有符号16位整型
using uint16    =unsigned short;        ///<无符号16位整型
using  int32    =  signed int;          ///<有符号32位整型
using uint32    =unsigned int;          ///<无符号32位整型
using  int64    =  signed long long;    ///<有符号64位整型
using uint64    =unsigned long long;    ///<无符号64位整型

typedef unsigned long   ulong;
typedef unsigned int    uint;
typedef unsigned short  ushort;
typedef unsigned char   uchar;

/**
* 取适合正巧大于当前数的2次幂值
*/
template<typename T>
inline T power_to_2(T value)
{
    T result=1;

    while(result<value)
        result<<=1;

    return(result);
}

inline void replace(char *str,size_t str_len,const char new_char,const char old_char)
{
    while(str_len--)
    {
        if(*str==old_char)
            *str=new_char;

        ++str;
    }
}

inline void replace(std::string &str,const char new_char,const char old_char)
{
    for(std::string::iterator it=str.begin();it<str.end();it++)
    {
        if(*it==old_char)
            *it=new_char;
    }
}

/**
* 将一串原始数据转转成16进制数值字符串
* @param str 16进制数值字符串存入处
* @param src 原始的数据
* @param size 原始数据字节长度
*/
inline void DataToHexStr(char *str,const uint8 *src,const int size)
{
    constexpr char hexstr[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};    ///<大写16进制字符

    int i;

    for(i=0;i<size;i++)
    {
        *str=hexstr[((*src)&0xF0)>>4];
        ++str;
        *str=hexstr[ (*src)&0x0F    ];
        ++str;

        ++src;
    }

    *str=0;
}
