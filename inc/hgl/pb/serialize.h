#ifndef HGL_PB_SERIALIZE_INCLUDE
#define HGL_PB_SERIALIZE_INCLUDE

#include<hgl/MemBlock.h>
#include<hgl/io/OutputStream.h>
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

			Serialize()=default;
			virtual ~Serialize()=default;

			void ClearData()
			{
				mb.ClearData();
			}

			template<typename T> bool Append(const T *msg)
			{
				hgl::int32 size=msg->ByteSize();

				if(size<0)
					return(true);

				hgl::int32 start=mb.length();

				mb.AddLength(size+(sizeof(int32)*2));

				hgl::int32 *mb_int=(hgl::int32 *)(mb.data()+start);

				*mb_int=size+sizeof(hgl::int32);
				++mb_int;
				*mb_int=T::id;
				++mb_int;

				if(size==0)
					return(true);

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

				return(*this);
			}
		};//class Serialize

		/**
         * 序列化输出流
         */
        class SerializeOutputStream:public pb::Serialize
        {
            hgl::io::OutputStream *os;

        public:

            SerializeOutputStream(hgl::io::OutputStream *_os)
            {
                os=_os;
            }

            template<typename T> bool Write(const T *msg)
            {
                if(!os)
                    RETURN_FALSE;

                if(!Append(msg))
                {
                    this->mb.ClearData();
                    return(false);
                }

                if(os->WriteFully(this->mb.data(),this->mb.length())!=this->mb.length())
                    RETURN_FALSE;

                this->mb.ClearData();

                return(true);
            }
        };//class SerializeOutputStream
	}//namespace pb
}//namespace hgl
#endif//HGL_PB_SERIALIZE_INCLUDE
