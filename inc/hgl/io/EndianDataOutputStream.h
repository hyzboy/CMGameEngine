#ifndef HGL_IO_ENDIAN_DATA_OUTPUT_STREAM_INCLUDE
#define HGL_IO_ENDIAN_DATA_OUTPUT_STREAM_INCLUDE

#include<hgl/io/DataOutputStream.h>
#include<hgl/endian/Endian.h>
namespace hgl
{
	namespace io
	{
		class DirectDataOutputStream:public DataOutputStream
		{
		public:

			DirectDataOutputStream(OutputStream *os):DataOutputStream(os)
			{
			}

			virtual ~DirectDataOutputStream()
			{
			}

			#define STREAM_DIRECT_WRITE(type,name)	virtual bool Write##name(const type &value)	\
													{	\
														return Write(value);	\
													}	\
													\
													virtual int64 Write##name(const type *data,const int64 count)	\
													{	\
														if(count<=0)return(count);	\
														if(!data)return(-1);	\
														\
														return WriteArrays(data,count);	\
													}

			STREAM_DIRECT_WRITE(int16,	Int16	);
			STREAM_DIRECT_WRITE(int32,	Int32	);
			STREAM_DIRECT_WRITE(int64,	Int64	);

			STREAM_DIRECT_WRITE(uint16,	Uint16	);
			STREAM_DIRECT_WRITE(uint32,	Uint32	);
			STREAM_DIRECT_WRITE(uint64,	Uint64	);

			STREAM_DIRECT_WRITE(u16char,UTF16Char);
			STREAM_DIRECT_WRITE(float,	Float	);
			STREAM_DIRECT_WRITE(double,	Double	);

			#undef STREAM_DIRECT_WRITE
		};//class DirectDataOutputStream

		class SwapDataOutputStream:public DataOutputStream
		{
		public:

			SwapDataOutputStream(OutputStream *os):DataOutputStream(os)
			{
			}

			virtual ~SwapDataOutputStream()
			{
			}

			#define STREAM_SWAP_WRITE(type,name)	virtual bool Write##name(const type &value)	\
													{\
														return Write(EndianSwap(value));	\
													}	\
													\
													virtual int64 Write##name(const type *data,const int64 count)	\
													{	\
														if(count<=0)return(count);	\
														if(!data)return(-1);	\
														\
														SharedArray<type> swap_data=new type[count];	\
														\
														EndianSwap<type>(swap_data,data,count);	\
														\
														return WriteArrays<type>(swap_data,count);	\
													}

			STREAM_SWAP_WRITE(int16,	Int16	);
			STREAM_SWAP_WRITE(int32,	Int32	);
			STREAM_SWAP_WRITE(int64,	Int64	);

			STREAM_SWAP_WRITE(uint16,	Uint16	);
			STREAM_SWAP_WRITE(uint32,	Uint32	);
			STREAM_SWAP_WRITE(uint64,	Uint64	);

			STREAM_SWAP_WRITE(u16char,	UTF16Char);
			STREAM_SWAP_WRITE(float,	Float	);
			STREAM_SWAP_WRITE(double,	Double	);

			#undef STREAM_SWAP_WRITE
		};//class SwapDataOutputStream

#if HGL_ENDIAN == HGL_LITTLE_ENDIAN
		typedef DirectDataOutputStream	LEDataOutputStream;
		typedef SwapDataOutputStream	BEDataOutputStream;
#else
		typedef DirectDataOutputStream	BEDataOutputStream;
		typedef SwapDataOutputStream	LEDataOutputStream;
#endif//HGL_ENDIAN == HGL_LITTLE_ENDIAN
	}//namespace io
}//namespace hgl
#endif//HGL_IO_ENDIAN_DATA_OUTPUT_STREAM_INCLUDE
