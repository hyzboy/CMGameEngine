#include<hgl/thread/RingBuffer.h>
#include<string.h>

namespace hgl
{
	/**
	* 安全取得可读取数据长度
	*/
	int RingBuffer::SafeGetReadSize()
	{
		Lock();

		const int result=_GetReadSize();

		Unlock();

		return(result);
	}

	/**
	 * 安全尝试取得可读取数据长度
	 */
	bool RingBuffer::SafeTryGetReadSize(int &result)
	{
		if(!TryLock())
			return(false);

		result=_GetReadSize();

		Unlock();

		return(true);
	}

	int RingBuffer::ReadStart(bool peek)
	{
		ClampPosition();

		write_off=write_pos%buffer_size;

		read_max=_GetReadSize();

		read_peek=peek;				//标记是否真的取走
		read_count=0;
		read_cur=read_pos;

		return(read_max);
	}

	int RingBuffer::_SafeReadStart(bool peek)
	{
		ReadStart(peek);

		if(read_max<=0)
			Unlock();

		return(read_max);
	}

	/**
	* 尝试开始读取数据，如失败或没有数据会立即解锁，不必再次调用SafeReadEnd
	* @param peek 是否真的取出数据
	* @return >0 可供读取的数据数量
	* @return =0 没有可供读取的数据
	* @return <0 暂时不能读取
	*/
	int RingBuffer::SafeTryReadStart(bool peek)
	{
		if(!TryLock())
			return(-1);

		return _SafeReadStart(peek);
	}

	/**
	* 开始读取数据,如果没有数据会立即关闭缓冲区，不必再次调用SafeReadEnd
	* @param peek 是否真的取出数据
	* @return 可供读取的数据数量
	*/
	int RingBuffer::SafeReadStart(bool peek)
	{
		Lock();

		return _SafeReadStart(peek);
	}

	/**
	* 读取数据
	* @param data 欲将访问数据存放的内存地址,可为NULL用于单纯后移读取指针
	* @param size 欲访问的数据长度
	* @param peek 是否后移访问指针(默认为真，仅针对当前这一次)
	* @return 实际可访问的数据长度
	*/
	int RingBuffer::Read(void *data,int size,bool peek)
	{
		if(size<=0)return(-1);

		const int result=_Read(data,size,peek);

		if(peek)
			read_count+=result;

		return(result);
	}

	/**
	* 读取结束
	* @return 返回读取的数据长度
	*/
	int RingBuffer::ReadEnd()
	{
		const int result=read_count;

		if(read_peek&&result)
			read_pos=read_cur;

		return result;
	}

	/**
	* 安全读取结束
	* @return 返回读取的数据长度
	*/
	int RingBuffer::SafeReadEnd()
	{
		const int result=ReadEnd();

		Unlock();

		return result;
	}

	/**
	* 安全的读取数据,,此函数会直接开锁解锁，用于少量的一次性处理。如大量的数据要分次读取，请使用SafeReadStart/SafeReadEnd
	* @param data 欲将访问数据存放的内存地址,可为NULL用于单纯后移读取指针
	* @param size 欲访问的数据长度
	* @param peek 是否真的取出数据
	* @return 实际读取的数据长度
	* @return -1 出错
	*/
	int RingBuffer::SafeRead(void *data,int size,bool peek)
	{
		if(size<0)return(-1);
		if(size&&!data)return(-1);

		if(SafeReadStart(peek)<=0)
			return(-1);

		Read(data,size,peek);

		return SafeReadEnd();
	}

	int RingBuffer::_Read(void *data,int size,bool peek)
	{
		if(size<=0||read_max<=0)return(0);

		if(size>read_max)
			size=read_max;

		if(size>0)
		{
			if(data)	//data可以为空，用于先尝试取数据后直接移走
			{
				const int temp_read =read_cur %buffer_size;

				if(temp_read<write_off)
				{
					memcpy(data,buffer+temp_read,size);
				}
				else
				{
					int temp=buffer_size-temp_read;

					if(size>temp)
					{
						memcpy(data,buffer+temp_read,temp);

						memcpy(((char *)data)+temp,buffer,size-temp);
					}
					else
					{
						memcpy(data,buffer+temp_read,size);
					}
				}
			}//if(data)

			if(peek)
			{
				read_cur+=size;
				read_max-=size;
			}
		}

		return(size);
	}
}//namespace hgl
