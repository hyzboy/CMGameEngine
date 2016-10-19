#ifndef HGL_IO_TYPE_INCLUDE
#define HGL_IO_TYPE_INCLUDE

#include<hgl/platform/Platform.h>
namespace hgl
{
	namespace io
	{
		class DataInputStream;
		class DataOutputStream;

		template<typename T> struct io_type
		{
			T value;

		public:

			io_type()=default;
			io_type(const T &v){value=v;}

			bool Read(DataInputStream *);
			bool Write(DataOutputStream *)const;

			void operator = (const T &v)
			{
				value=v;
			}

			operator T (){return value;}
			operator const T ()const{return value;}
		};//template<typename T> struct io_type
	}//namespace io
}//namespace hgl
#endif//HGL_IO_TYPE_INCLUDE
