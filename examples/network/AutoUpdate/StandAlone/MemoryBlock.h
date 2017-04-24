#pragma once

#include<string.h>
template<typename T> class MemoryBlock
{
    T *mb;
    size_t size;

public:

    MemoryBlock(const size_t &s=0)
    {
        if(s>0)
        {
            mb=new T[s];

            if(mb)
            {
                size=s;
                return;
            }
        }

        mb=nullptr;
        size=0;
    }

    ~MemoryBlock()
    {
        delete[] mb;        //delete[] nullptr不是个错误
    }

    const size_t length()const
    {
        return size;
    }

    const size_t bytes()const
    {
        return size*sizeof(T);
    }

    void zero()
    {
        if(!mb)return;

        memset(mb,0,size*sizeof(T));
    }

    bool realloc(const size_t &s)
    {
        if(size>=s)return(true);

        T *nb=new T[s];

        if(nb)
        {
            memcpy(nb,mb,size*sizeof(T));

            delete[] mb;
            mb=nb;
            size=s;
            return(true);
        }

        return(false);
    }

    bool crealloc(const size_t &s)
    {
        if(size>=s)return(true);
        
        T *nb=new T[s];

        if(nb)
        {
            delete[] mb;
            mb=nb;
            size=s;
            return(true);
        }

        return(false);            
    }

    T *data(){return mb;}

    T &operator[](const int &index)
    {
        return  mb[index];
    }

    operator const T *()const{return mb;}
    operator T *(){return mb;}
};//template<typename T> class MemoryBlock

