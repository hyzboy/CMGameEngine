#ifndef HGL_IMAGE_EXT_INCLUDE
#define HGL_IMAGE_EXT_INCLUDE

#include<hgl/graph/TextureFormat.h>
#include<hgl/type/BaseString.h>
namespace hgl
{
	namespace graph
	{
		const char Tex2DFlag[]		="Tex2D\x1A";					//2D贴图文件标识头
		const uint Tex2DFlagLength	=sizeof(Tex2DFlag)-1;			//2D贴图文件标识头长度
		const uint Tex2DVerLength	=sizeof(float);					//2D贴图文件标识版本号数据长度
		const uint Tex2DHeaderLength=Tex2DFlagLength+Tex2DVerLength;

		struct Tex2DFileHeader
		{
			uint32 header_size;			//文件头长度
			uint32 width;
			uint32 height;
			char format[16];			//显存格式
			uint32 bytes;
		};//struct Tex2DFileHeader

		/**
		* 2D位图数据处理类
		*/
		class Bitmap2D
		{
			uint8 *data;

			Tex2DFileHeader *header;

			TextureSourceFormat format;

		public:

			uint32				GetWidth		()const{return header?header->width:0;}
			uint32				GetHeight		()const{return header?header->height:0;}
			TextureSourceFormat	GetFormat		()const{return format;}
			void *				GetData			()const{return header?data+Tex2DHeaderLength+header->header_size:0;}
			uint32				GetDataLength	()const{return header?header->bytes:0;}

		public:

			Bitmap2D()
			{
				data=0;
				header=0;
			}

			virtual ~Bitmap2D()
			{
				delete[] data;
			}

			virtual bool LoadFromTextureFile(const OSString &);
		};//class Bitmap2D
	}//namespace graph

	namespace graph
	{
		void L2LA(uint8 *,uint8 *,uint8 *,uint32);
		void RGB2RGBA(uint8 *,uint8 *,uint8 *,uint32);
		void RGBA2RGB(uint8 *,uint8 *,uint32);
		void RGBA2RGB_A(uint8 *,uint8 *,uint8 *,uint32);

		bool CheckMono(uint8 *,uint,uint,uint);
		void *ConvertToMono(uint8 *,uint,uint,uint);

		int CheckAlpha(uint8 *,uint,uint,uint);
		uint8 *ClearAlpha(uint8 *,uint,uint,uint);

		//void ClearColor(uchar *,uint,uint);
		//void ClearColor(uchar *,uchar *,uint,uint);

		void SplitToChannels(uint8 *,uint8 *,int,int);
	}//namespace graph
}//namespace hgl
#endif//HGL_IMAGE_EXT_INCLUDE
