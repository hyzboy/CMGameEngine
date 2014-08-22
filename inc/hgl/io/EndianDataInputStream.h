#ifndef HGL_IO_ENDIAN_DATA_INPUT_STREAM_INCLUDE
#define HGL_IO_ENDIAN_DATA_INPUT_STREAM_INCLUDE

#include<hgl/io/DataInputStream.h>
#include<hgl/endian/Endian.h>
namespace hgl
{
	namespace io
	{
		class DirectDataInputStream:public DataInputStream
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using DataInputStream::DataInputStream;
#else
			DirectDataInputStream(InputStream *is):DataInputStream(is){}
#endif//HGL_CONSTRUCTION_REUSE
			virtual ~DirectDataInputStream()HGL_DEFAULT_MEMFUNC;

			#define STREAM_DIRECT_READ(type,name)	virtual bool Read##name(type &value)	\
													{	\
														return Read(value);	\
													}	\
													\
													virtual int64 Read##name(type *data,const int64 count)	\
													{	\
														if(count<=0)return(count);	\
														if(!data)return(-1);	\
														\
														return ReadArrays(data,count);	\
													}

			STREAM_DIRECT_READ(int16,	Int16	);
			STREAM_DIRECT_READ(int32,	Int32	);
			STREAM_DIRECT_READ(int64,	Int64	);

			STREAM_DIRECT_READ(uint16,	Uint16	);
			STREAM_DIRECT_READ(uint32,	Uint32	);
			STREAM_DIRECT_READ(uint64,	Uint64	);

			STREAM_DIRECT_READ(u16char,UTF16Char);
			STREAM_DIRECT_READ(float,	Float	);
			STREAM_DIRECT_READ(double,	Double	);

			#undef STREAM_DIRECT_READ
		};//class DirectDataInputStream

		class SwapDataInputStream:public DataInputStream
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using DataInputStream::DataInputStream;
#else
			SwapDataInputStream(InputStream* is):DataInputStream(is){}
#endif//HGL_CONSTRUCTION_REUSE
			virtual ~SwapDataInputStream()HGL_DEFAULT_MEMFUNC;

			#define STREAM_SWAP_READ(type,name) virtual bool Read##name(type &value)	\
												{\
													type swap_value;	\
													\
													if(!Read(swap_value))	\
														return(false);	\
												\
													value=EndianSwap(swap_value);	\
													return(true);	\
												}	\
												\
												virtual int64 Read##name(type *data,const int64 count)	\
												{	\
													if(count<=0)return(count);	\
													if(!data)return(-1);	\
													\
													const int64 result=ReadArrays(data,count);	\
													\
													if(result<=0)return(result);	\
													\
													EndianSwap(data,result);	\
													return(result);	\
												}

			STREAM_SWAP_READ(int16,		Int16	);
			STREAM_SWAP_READ(int32,		Int32	);
			STREAM_SWAP_READ(int64,		Int64	);

			STREAM_SWAP_READ(uint16,	Uint16	);
			STREAM_SWAP_READ(uint32,	Uint32	);
			STREAM_SWAP_READ(uint64,	Uint64	);

			STREAM_SWAP_READ(u16char,	UTF16Char);
			STREAM_SWAP_READ(float,		Float	);
			STREAM_SWAP_READ(double,	Double	);

			#undef STREAM_SWAP_READ
		};//class SwapDataInputStream

#if HGL_ENDIAN == HGL_LITTLE_ENDIAN
		typedef DirectDataInputStream	LEDataInputStream;
		typedef SwapDataInputStream		BEDataInputStream;
#else
		typedef DirectDataInputStream	BEDataInputStream;
		typedef SwapDataInputStream		LEDataInputStream;
#endif//HGL_ENDIAN == HGL_LITTLE_ENDIAN
	}//namespace io
}//namespace hgl
#endif//HGL_IO_ENDIAN_DATA_INPUT_STREAM_INCLUDE
