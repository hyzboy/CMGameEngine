#include<hgl/io/IOType.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
namespace hgl
{
    namespace io
    {
#define IO_TYPE_DEFINE(small,large)    typedef io_type<small> io##large;    \
                                    \
                                    template<> bool io_type<small>::Read(hgl::io::DataInputStream *dis)    \
                                    {    \
                                        if(!dis)return(false);    \
                                        return dis->Read##large(value);    \
                                    }    \
                                    \
                                    template<> bool io_type<small>::Write(hgl::io::DataOutputStream *dos)const    \
                                    {    \
                                        if(!dos)return(false);    \
                                        return dos->Write##large(value);    \
                                    }

    IO_TYPE_DEFINE(int8,Int8);
    IO_TYPE_DEFINE(int16,Int16);
    IO_TYPE_DEFINE(int32,Int32);
    IO_TYPE_DEFINE(int64,Int64);

    IO_TYPE_DEFINE(uint8,Uint8);
    IO_TYPE_DEFINE(uint16,Uint16);
    IO_TYPE_DEFINE(uint32,Uint32);
    IO_TYPE_DEFINE(uint64,Uint64);

    IO_TYPE_DEFINE(bool,Bool);
    IO_TYPE_DEFINE(float,Float);
    IO_TYPE_DEFINE(double,Double);
#undef IO_TYPE_DEFINE
    }//namespace io
}//namespace hgl
