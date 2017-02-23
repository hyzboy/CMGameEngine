#include<hgl/type/BitArray.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<string.h>

namespace hgl
{
    void BitArray::InitPrivate()
    {
        hglSetPropertyRead( ByteSize,   this,BitArray::GetByteSize                      );
        hglSetProperty(     Count,        this,BitArray::GetCount,    BitArray::SetCount  );
        hglSetProperty(     Position,    this,BitArray::GetPos,        BitArray::SetPos    );
        hglSetPropertyRead( Data,        this,BitArray::GetData                          );
    }

    /**
    * 本类构造函数，并创建指定长度的数据空间
    * @param bitsize 位长度
    */
    BitArray::BitArray(int bitsize)
    {
        InitPrivate();

        data=nullptr;

        Create(bitsize);
    }

    /**
    * 本类构造函数,并传入部分数据,做为已有的数据
    * @param bytedata 原有的数据
    * @param size 数据字节数
    * @param 是否是映射的其它数据区，如果为true，则在本类被清除时不会释放数据区
    */
    BitArray::BitArray(void *bytedata,int size,bool map)
    {
        InitPrivate();

        if(size>0)
        {
            byte_size=size;
            count=byte_size<<3;
            pos=0;

            mapping=map;

            if(mapping)
            {
                data=(uint8 *)bytedata;
            }
            else
            {
                data=(uint8 *)hgl_malloc(byte_size);
                memcpy(data,bytedata,size);
            }
        }
        else
        {
            byte_size=0;
            count=0;
            pos=0;

            data=nullptr;

            mapping=false;
        }
    }

    BitArray::~BitArray()
    {
        Clear();
    }

    void BitArray::MapData(void *bytedata,int size)
    {
        Clear();

        if(size>0)
        {
            byte_size=size;
            count=byte_size<<3;
            pos=0;

            mapping=true;
            data=(uint8 *)bytedata;
        }
    }

    void BitArray::Clear()
    {
        if(data)
        {
            if(!mapping)
                hgl_free(data);

            data=nullptr;
        }

        mapping=false;

        count=0;
        pos=0;
        byte_size=0;
    }

    /**
    * 创建指定长度的数据空间
    * @param bitsize 位长度
    */
    void BitArray::Create(int bitsize)
    {
        Clear();

        if(bitsize>0)
        {
            byte_size=(bitsize+7)>>3;
            count=bitsize;

            data=(uint8 *)hgl_malloc(byte_size);
            memset(data,0,byte_size);
        }
        else
        {
            byte_size=0;
            count=0;

            data=nullptr;
        }

        pos=0;
    }

    void BitArray::SetCount(int newcount)
    {
        if(newcount==count)return;
        if(newcount<=0)
        {
            Clear();
            return;
        }

        int new_size=(newcount+7)>>3;

        if(byte_size==0)
        {
            byte_size=new_size;
            count=newcount;
            pos=0;

            data=(uint8 *)hgl_malloc(byte_size);
        }
        else
        {
            if(byte_size!=new_size)
            {
                byte_size=new_size;

                data=(uint8 *)hgl_realloc(data,new_size);
            }

            count=newcount;
            if(pos>count)pos=count;
        }
    }

    void BitArray::SetPos(int npos)
    {
        if(npos<0)pos=0;else
        if(npos>count)pos=count;else
        pos=npos;
    }

    /**
    * 全部填充成指定数据
    */
    void BitArray::Fill(bool v)
    {
        if(data)
            memset(data,v?255:0,byte_size);
    }

    /**
    * 向位队列中写入一位0
    */
    bool BitArray::WriteZero()
    {
        if(!data)return(false);

        int byte_offset;
        int bit_offset;

        if(pos==count)
            count++;

        byte_offset=pos>>3;        //除8
        bit_offset=pos&7;          //取最后3位，最大是7

        data[byte_offset]&=0xFF^(128>>bit_offset);

        pos++;
        return(true);
    }

    /**
    * 向位队列中写入一位1
    */
    bool BitArray::WriteOne()
    {
        if(!data)return(false);

        int byte_offset;
        int bit_offset;

        if(pos==count)
            count++;

        byte_offset=pos>>3;        //除8
        bit_offset=pos&7;          //取最后3位，最大是7

        data[byte_offset]|=128>>bit_offset;

        pos++;
        return(true);
    }

    /**
    * 向位队列中写入一位数据
    * @param off 位置
    * @param bit 数据
    * @return 是否写入成功
    */
    bool BitArray::WritePos(int off,bool bit)
    {
        if(!data)return(false);

        int byte_offset;
        int bit_offset;

        byte_offset=off>>3;        //除8
        bit_offset=off&7;          //取最后3位，最大是7

        if(bit)
            data[byte_offset]|=128>>bit_offset;
        else
            data[byte_offset]&=0xFF^(128>>bit_offset);

        return(true);
    }

    /**
    * 按字节向队列写入数据
    * @param bytedata 字节数据
    * @param size 字节长度
    */
    bool BitArray::WriteByte(uint8 *bytedata,int size)
    {
        if(!data)return(false);

        int byte_offset,n;
        int bit_offset;

        if(pos+(size<<3)>count)
            count=pos+(size<<3);

        byte_offset=pos>>3;
        bit_offset=pos&7;

        pos+=size<<3;

        if(bit_offset==0)           //当位正好对齐时直接复制数据
        {
            memcpy(data+byte_offset,bytedata,size);
        }
        else
        {
            while(size--)
            {
                n=0;
                do
                {
                    if(*bytedata&(128>>n))
                        data[byte_offset]|=128>>bit_offset;
                    else
                        data[byte_offset]&=0xFF^(128>>bit_offset);

                    if(++bit_offset==8)
                    {
                        byte_offset++;
                        bit_offset=0;
                    }
                }while(++n<8);

                bytedata++;
            }
        }

        return(true);
    }

    /**
    * 从位队列中读取一位
    * @param bit 读取出来的位数据
    * @return 是否读取成功
    */
    bool BitArray::ReadBit(bool &bit)
    {
        if(!data)return(false);

        int byte_offset;
        int bit_offset;

        byte_offset=pos>>3;        //除8
        bit_offset=pos&7;          //取最后3位，最大是7

        pos++;

        bit=data[byte_offset]&(128>>bit_offset);

        return(true);
    }

    /**
    * 从位队列中读取一位
    * @param off 位置
    * @param bit 读取出来的位数据
    * @return 是否读取成功
    */
    bool BitArray::ReadPos(int off,bool &bit)const
    {
        if(!data)return(false);

        int byte_offset;
        int bit_offset;

        byte_offset=off>>3;        //除8
        bit_offset=off&7;          //取最后3位，最大是7

        bit=data[byte_offset]&(128>>bit_offset);

        return(true);
    }

    /**
    * 翻转一位数据，将指0变1，1变0
    * @return 翻转后的数据
    */
    bool BitArray::FlipBit()
    {
        int byte_offset;
        int bit_offset;

        byte_offset=pos;        //除8
        bit_offset=pos;          //取最后3位，最大是7

        pos++;

        data[byte_offset]^=(128>>bit_offset);

        return(data[byte_offset]&(128>>bit_offset));
    }

    BitArray &BitArray::operator =(const BitArray &rhs)
    {
        if(this!=&rhs)
        {
            Clear();

            if(rhs.count>0)
            {
                byte_size=rhs.byte_size;
                count=rhs.count;

                data=(uint8 *)hgl_malloc(byte_size);

                memcpy(data,rhs.data,byte_size);

                pos=0;
                mapping=false;
            }
        }

        return *this;
    }

    BitArray &BitArray::operator +=(const BitArray &rhs)
    {
        int n=count;
        int n8;
        uint8 bit=128;

        if(rhs.count<n)
            n=rhs.Count;

        n8=n>>3;

        for(int i=0;i<n8;i++)
            data[i]|=rhs.data[i];

        for(int i=n8<<3;i<n;i++)
        {
            data[n8]|=rhs.Data[n8]&bit;
            bit>>=1;
        }

        return *this;
    }

    BitArray &BitArray::operator -=(const BitArray &rhs)
    {
        int n=count;
        int n8;
        uint8 bit=128;

        if(rhs.count<n)
            n=rhs.Count;

        n8=n>>3;

        for(int i=0;i<n8;i++)
            data[i]^=rhs.data[i];

        for(int i=n8<<3;i<n;i++)
        {
            data[n8]^=rhs.Data[n8]&bit;
            bit>>=1;
        }

        return *this;
    }

    BitArray &BitArray::operator *=(const BitArray &rhs)
    {
        int n=count;
        int n8;
        uint8 bit=128;

        if(rhs.count<n)
            n=rhs.Count;

        n8=n>>3;

        for(int i=0;i<n8;i++)
            data[i]&=rhs.data[i];

        for(int i=n8<<3;i<n;i++)
        {
            data[n8]&=rhs.Data[n8]&bit;
            bit>>=1;
        }

        return *this;
    }

    bool BitArray::operator == (const BitArray &rhs) const
    {
        int n8=count>>3;
        uint8 bit=128;

        if(memcmp(data,rhs.Data,n8)==0)
        {
            for(int i=n8<<3;i<count;i++)
                if((data[n8]&bit)!=(rhs.Data[n8]&bit))return(false);

            return(true);
        }

        return(false);
    }

    bool BitArray::SaveToStream(io::DataOutputStream *str)
    {
        if(str->WriteInt32(count))
            if(str->Write(data,byte_size))
                return(true);

        return(false);
    }

    bool BitArray::LoadFromStream(io::DataInputStream *str)
    {
        Clear();

        int tcount;

        if(str->ReadInt32(tcount))
        {
            Create(tcount);

            if(str->Read(data,byte_size))
                return(true);
        }

        return(false);
    }
}//namespace hgl
