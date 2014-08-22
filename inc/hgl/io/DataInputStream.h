#ifndef HGL_IO_DATA_INPUT_STREAM_INCLUDE
#define HGL_IO_DATA_INPUT_STREAM_INCLUDE

#include<hgl/io/InputStream.h>
#include<hgl/type/BaseString.h>
#include<hgl/VectorMath.h>
namespace hgl
{
	namespace io
	{
		class InputStream;

		/**
		* 格式数据输入流基类
		*/
		class DataInputStream																		///格式数据输入流基类
		{
		protected:

			InputStream *in;

		public:

			DataInputStream(InputStream *is)
			{
				in=is;
			}

			virtual ~DataInputStream()
			{
			}

			virtual void Use(InputStream *is)
			{
				in=is;
			}

			virtual int64 Read(void *buf,int64 size)
			{
				if(!buf||size<0)return(-1);

				return(in?in->Read(buf,size):-1);
			}

			virtual int64 Peek(void *buf,int64 size)
			{
				if(!buf||size<0)return(-1);

				return(in?in->Peek(buf,size):-1);
			}

			virtual int64 ReadFully(void *buf,int64 size)
			{
				if(!buf||size<0)return(-1);

				return(in?in->ReadFully(buf,size):-1);
			}

			virtual int64 Seek(int64 offset,SeekOrigin so=soBegin)
			{
				return(in?in->Seek(offset,so):-1);
			}

			virtual int64	Skip(int64 size)														///<跳过指定字节不访问
			{
				return(in?in->Skip(size):-1);
			}

			virtual bool	CanRestart	()const{return in?in->CanRestart():false;}					///<是否可以复位
			virtual bool	CanSeek		()const{return in?in->CanSeek	():false;}					///<是否可以定位
			virtual bool	CanSize		()const{return in?in->CanSize	():false;}					///<是否可以取得尺寸
			virtual bool	CanPeek		()const{return in?in->CanPeek	():false;}					///<是否可以预览数据

			virtual bool	Restart		()const{return in?in->Restart	():false;}					///<复位访问指针
			virtual int64	Tell		()const{return in?in->Tell		():-1;}						///<返回当前访问位置
			virtual int64	GetSize		()const{return in?in->GetSize	():-1;}						///<取得流长度
			virtual int64	Available	()const{return in?in->Available	():-1;}						///<剩下的可以不受阻塞读取的字节数

			#define STREAM_READ(type,name)	virtual bool Read##name(type &value)	\
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

			STREAM_READ(char,	Char	);
			STREAM_READ(bool,	Bool	);
			STREAM_READ(int8,	Int8	);
			STREAM_READ(uint8,	Uint8	);

			#undef STREAM_READ

			#define STREAM_READ(type,name)	virtual bool Read##name(type &)=0;	\
											virtual int64 Read##name(type *,const int64)=0;

			STREAM_READ(int16,		Int16	);
			STREAM_READ(int32,		Int32	);
			STREAM_READ(int64,		Int64	);

			STREAM_READ(uint16,		Uint16	);
			STREAM_READ(uint32,		Uint32	);
			STREAM_READ(uint64,		Uint64	);

			STREAM_READ(u16char,	UTF16Char);
			STREAM_READ(float,		Float	);
			STREAM_READ(double,		Double	);

			#undef STREAM_READ

			bool Read(Vector2f &vec){return ReadFloat((float *)&vec,2)==2;}
			bool Read(Vector3f &vec){return ReadFloat((float *)&vec,3)==3;}
			bool Read(Vector4f &vec){return ReadFloat((float *)&vec,4)==4;}

			/**
			* 自适应类型数据读取</p>
			* 请在多平台混用的情况下不要使用此函数，以免造成不同平台数据实质结构不一致的情况
			*/
			template<typename T> bool Read(T &data)
			{
				return(ReadFully(&data,sizeof(T))==sizeof(T));
			}

			/**
			* 自适应类型数据阵列读取</p>
			* 请在多平台混用的情况下不要使用此函数，以免造成不同平台数据实质结构不一致的情况
			* @param data 数据存放区
			* @param count 数据个数
			* @return 实质读入的数据个数
			* @return <0 出错
			*/
			template<typename T> int64 ReadArrays(T *data,int64 count)
			{
				if(count<=0)return(count);
				if(!data)return(-1);

				return ReadFully(data,count*sizeof(T))/sizeof(T);
			}

			/**
			* 自定义类型数据阵列读取</p>
			* 请在多平台混用的情况下不要使用此函数，以免造成不同平台数据实质结构不一致的情况
			* @param count 数据个数
			* @param alloc_count 分配数据个数(默认-1)
			* @return 实质读入的数据指针，请使用delete[]释放
			* @return NULL 出错
			*/
			template<typename T> T *ReadArrays(int64 count)
			{
				if(count<=0)return(nullptr);

				T *data=new T[count];

				if(data==0)
					return(nullptr);

				if(ReadArrays(data,count)!=count)
				{
					delete[] data;
					return(nullptr);
				}

				return data;
			}

			virtual bool ReadUTF16LEChars(u16char *,int);											///<读取utf16-le格式字符阵列到u16char *
			virtual bool ReadUTF16BEChars(u16char *,int);											///<读取utf16-be格式字符阵列到u16char *

			virtual bool ReadUTF8String(char *,int max_len=0);
			virtual bool ReadUTF16LEString(u16char *,int max_len=0);
			virtual bool ReadUTF16BEString(u16char *,int max_len=0);

			virtual bool ReadUTF8String(UTF8String &,int max_len=0);								///<读取utf8格式字符串(前缀四字节的字符串字节长度)
			virtual bool ReadUTF8String(UTF16String &,int max_len=0);								///<读取utf8格式字符串(前缀四字节的字符串字节长度)
			virtual bool ReadUTF16LEString(UTF16String &,int max_len=0);							///<读取utf16-le格式字符串(前缀四字节的字符串字节长度)
			virtual bool ReadUTF16BEString(UTF16String &,int max_len=0);							///<读取utf16-be格式字符串(前缀四字节的字符串字节长度)
		};//class DataInputStream
	}//namespace io
}//namespace hgl
#include<hgl/io/EndianDataInputStream.h>
#endif//HGL_IO_DATA_INPUT_STREAM_INCLUDE
