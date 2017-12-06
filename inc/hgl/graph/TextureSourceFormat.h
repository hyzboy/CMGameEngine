#ifndef HGL_GRAPH_TEXTURE_SOURCE_FORMAT_INCLUDE
#define HGL_GRAPH_TEXTURE_SOURCE_FORMAT_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
	namespace graph
	{
		/**
		* 贴图数据源格式
		*/
		enum TextureSourceFormat:uint
		{
			HGL_SF_NONE=0,			//无用，做为枚举起始

			HGL_SF_R8,
			HGL_SF_RG8,
			HGL_SF_RGB8,
			HGL_SF_RGBA8,

			HGL_SF_SRGB8,
			HGL_SF_SRGBA8,

			HGL_SF_R16,
			HGL_SF_RG16,
			HGL_SF_RGB16,
			HGL_SF_RGBA16,

			HGL_SF_R16F,
			HGL_SF_RG16F,
			HGL_SF_RGB16F,
			HGL_SF_RGBA16F,

			HGL_SF_R32F,
			HGL_SF_RG32F,
			HGL_SF_RGB32F,
			HGL_SF_RGBA32F,

			HGL_SF_UNCOMPRESSED,	//无用，做为非压缩色分隔

			HGL_SF_R3_G3_B2,
            HGL_SF_RGB565,

			HGL_SF_RGBA4,
			HGL_SF_RGB5_A1,
			HGL_SF_RGB10_A2,

			HGL_SF_RG11F_B10F,
			HGL_SF_RGB9_E5,

			HGL_SF_DEPTH,			//无用，做为深度分隔

			HGL_SF_DEPTH16,
			HGL_SF_DEPTH24,
			HGL_SF_DEPTH32,

			HGL_SF_DEPTH32F,

			HGL_SF_INDEX,			//无用，做为索引色分隔

			HGL_SF_INDEX_16_RGB,
			HGL_SF_INDEX_16_RGBA,
			HGL_SF_INDEX_256_RGB,
			HGL_SF_INDEX_256_RGBA,

			HGL_SF_COMPRESSED,		//无用，做为压缩格式分隔

			//HGL_SF_CR,
			//HGL_SF_CRG,
			//HGL_SF_CRGB,
			//HGL_SF_CRGBA,

			//HGL_SF_CSRGB,
			//HGL_SF_CSRGBA,

			HGL_SF_DXT1RGB,
			HGL_SF_DXT1RGBA,
			HGL_SF_DXT3RGBA,
			HGL_SF_DXT5RGBA,

			HGL_SF_LATC1,
			HGL_SF_LATC1s,
			HGL_SF_LATC2,
			HGL_SF_LATC2s,

			HGL_SF_RGTC1,
			HGL_SF_RGTC1s,
			HGL_SF_RGTC2,
			HGL_SF_RGTC2s,

			HGL_SF_BPTC_RGBf,
			HGL_SF_BPTC_RGBuf,
			HGL_SF_BPTC_RGBA,
			HGL_SF_BPTC_SRGBA,

			HGL_SF_ETC2_RGB8,
			HGL_SF_ETC2_SRGB8,
			HGL_SF_ETC2_RGB8A1,
			HGL_SF_ETC2_SRGB8A1,
			HGL_SF_ETC2_RGBA8,
			HGL_SF_ETC2_SRGBA8,

			HGL_SF_EAC_R11,
			HGL_SF_EAC_R11s,
			HGL_SF_EAC_RG11,
			HGL_SF_EAC_RG11s,

			HGL_SF_END				//无用，做为枚举结束
		};//enum TextureSourceFormat

		using TSF=TextureSourceFormat;

		bool TextureSourceFormatCheck(const TextureSourceFormat &);
		bool TextureSourceFormatDepthCheck(const TextureSourceFormat &);

        struct TextureFormat
        {
            TextureSourceFormat tsf;    //数据源格式枚举

            char name[16];              //简写名称

            bool compress;              //是否压缩格式
            uint index;                 //索引色数量

            uint video_format;          //显存格式

            uint pixel_format;          //象素格式(指R/RG/RGB/RGBA/DEPTH这些)
            uint data_type;             //数据类型(指BYTE，SHORT，FLOAT这些)

            uint source_bytes;          //原始格式字节数
            uint video_bytes;           //显存格式字节数
        };

        //贴图数据源格式信息
        extern const TextureFormat TextureFormatInfoList[HGL_SF_END];

        TSF GetTextureFormatEnum(const char *);       //根据简写名称取得对应的TextureSourceFormat
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_SOURCE_FORMAT_INCLUDE
