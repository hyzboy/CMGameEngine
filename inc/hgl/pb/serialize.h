#ifndef HGL_PB_SERIALIZE_INCLUDE
#define HGL_PB_SERIALIZE_INCLUDE

#include<hgl/MemBlock.h>
namespace hgl
{
	namespace pb
	{
		/**
		* 序列化控制
		*/
		class Serialize
		{
		protected:

			MemBlock<char> mb;

		public:

			Serialize()HGL_DEFAULT_MEMFUNC;
			virtual ~Serialize()HGL_DEFAULT_MEMFUNC;

			void ClearData()
			{
				mb.ClearData();
			}

			template<typename T> bool Append(const T *msg)
			{
				hgl::int32 size=msg->ByteSize();

				if(size<=0)
					return(true);

				hgl::int32 start=mb.length();

				mb.AddLength(size+(sizeof(int32)*2));

				hgl::int32 *mb_int=(hgl::int32 *)(mb.data()+start);

				*mb_int=T::id;
				++mb_int;
				*mb_int=size;
				++mb_int;

				if(!msg->SerializeToArray(mb_int,size))
				{
					mb.SetLength(start);
					RETURN_FALSE;
				}

				return(true);
			}

			template<typename T>
			Serialize &operator << (const T *msg)
			{
				Append(msg);
			}
		};//class Serialize
	}//namespace pb
}//namespace hgl
#endif//HGL_PB_SERIALIZE_INCLUDE
