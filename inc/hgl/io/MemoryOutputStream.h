#ifndef HGL_IO_MEMORY_OUTPUT_STREAM_INCLUDE
#define HGL_IO_MEMORY_OUTPUT_STREAM_INCLUDE

#include<hgl/type/MemBlock.h>
#include<hgl/io/OutputStream.h>
namespace hgl
{
	namespace io
	{
		/**
		 * 内存数据输出流，将数据输出到一个内存块中。
		 */
		class MemoryOutputStream:public OutputStream												///内存数据输出流
		{
		protected:

			uint8 *buf;

			size_t max_size;
			size_t buf_size;
			size_t cur_pos;

			bool one;

		public:

			MemoryOutputStream()
			{
				buf=0;
				max_size=0;
				buf_size=0;
				cur_pos=0;
				one=false;
			}

			virtual ~MemoryOutputStream()
			{
				Close();
			}

			void *GetData()const{return buf;}

			/**
			 * 关联一个数据区到当前输出流
			 * @param ptr 数据指针
			 * @param size 数据长度字节数
			 * @param one_instance 是否仅此一份实例(如果是，将由MemoryOutputStream类负责释放)
			 * @return 是否成功
			 */
			bool Link(void *ptr,size_t size,bool one_instance=false)
			{
				if(!ptr||!size)
					return(false);

				buf=(uint8 *)ptr;
				buf_size=size;
				max_size=size;
				cur_pos=0;

				one=one_instance;

				return(true);
			}

			/**
			 * 更新关联的数据区长度，不复位cur_pos
			 * @param ptr 数据指针
			 * @param size 数据长度字节数
			 * @return 是否成功
			 */
			bool Update(void *ptr,size_t size)
			{
				if(!ptr||!size)
					return(false);

				buf=(uint8 *)ptr;
				buf_size=size;
				max_size=0;

				return(true);
			}

			void Unlink()
			{
				buf=0;
				buf_size=0;
				max_size=0;
			}

			bool Create(int64 size)
			{
				Close();

				if(size<=0)
					return(false);

				buf=(uint8 *)hgl_malloc(size);

				if(!buf)
					return(false);

				one=true;
				cur_pos=0;
				buf_size=size;
				max_size=size;

				return(true);
			}

			void Close()
			{
				if(buf)
				{
					if(one)
						hgl_free(buf);

					buf=0;
				}

				buf_size=0;
				max_size=0;
			}

			void ClearData()
			{
				cur_pos=0;
				buf_size=0;
			}

			int64 Write(const void *ptr,int64 size)
			{
				if(!ptr||size<0)
					return(-1);

				if(!buf)
				{
					if(!Create(size))
						return(-1);
				}

				if(cur_pos+size>buf_size)
				{
					if(one)
					{
						buf_size=cur_pos+size;

						if(buf_size>max_size)
						{
							max_size=power_to_2(buf_size);

							buf=(uint8 *)hgl_realloc(buf,max_size);
						}
					}
					else
						size=buf_size-cur_pos;
				}

				if(size<=0)
					return(0);

				memcpy(buf+cur_pos,ptr,size);

				cur_pos+=size;

				return size;
			}

			bool	CanRestart()const{return true;}
			bool	CanSeek()const{return true;}
			bool	CanSize()const{return true;}

			bool	Restart()
			{
				cur_pos=0;
				return(true);
			}

			int64	Seek(int64 off,SeekOrigin so=soBegin)
			{
				if(!CanSeek())return(-1);

				if(so==soCurrent)
				{
					off+=cur_pos;
				}
				else
				if(so==soEnd)
				{
					off+=buf_size;
				}

				if(off<0||off>=buf_size)
					return(-1);

				cur_pos=off;
				return cur_pos;
			}

			int64	Tell()const
			{
				return cur_pos;
			}

			int64	GetSize()const
			{
				return buf_size;
			}

			int64	Available()const
			{
				return buf_size-cur_pos;
			}
		};//class MemoryOutputStream
	}//namespace io
}//namespace hgl
#endif//HGL_IO_MEMORY_OUTPUT_STREAM_INCLUDE
