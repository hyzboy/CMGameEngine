#include<hgl/thread/RingBuffer.h>

/**
* 由于设定read_pos=write_pos表示无数据，所以当数据刚好读写到结尾时，不立即转成0，而是仍为buffer_size
*/
//--------------------------------------------------------------------------------------------------
namespace hgl
{
    RingBuffer::RingBuffer(int size)
    {
        buffer_size=size;

        buffer=new char[size];

//         memset(buffer,0,size);        //有write_pos/read_pos没有根本读不出数据，所以无需清0

        read_pos=0;
        write_pos=0;
    }

    RingBuffer::~RingBuffer()
    {
        delete[] buffer;
    }

    void RingBuffer::Clear()
    {
        read_pos=0;
        write_pos=0;
    }

    void RingBuffer::SafeClear()
    {
        Lock();
        Clear();
        Unlock();
    }

    void RingBuffer::ClampPosition()
    {
        if(read_pos<buffer_size
         ||write_pos<buffer_size)return;    //将指针减去整数部分，以免溢出

        //不可以用read_pos%=buffer_size，write_pos%=buffer_size。
        //因为有可能read_pos=0,write_pos=buffersize，都用%buffer_size就会形成两个都为0的错误。
        //一般也就破一次界限，所以效能影响无所谓。

        read_pos-=buffer_size;
        write_pos-=buffer_size;
    }
}//namespace hgl
