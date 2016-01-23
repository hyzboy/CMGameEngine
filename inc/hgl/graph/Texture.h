#ifndef HGL_GRAPH_TEXTURE_INCLUDE
#define HGL_GRAPH_TEXTURE_INCLUDE

#include<hgl/graph/TextureFormat.h>
#include<hgl/type/BaseString.h>
namespace hgl
{
	namespace graph
	{
		class Bitmap2D;

        extern int HGL_MAX_VS_TEXTURE_UNITS;        ///<Vertex Shader中的最大纹理单元
        extern int HGL_MAX_FS_TEXTURE_UNITS;        ///<Fragment Shader中的最大纹理单元
        extern int HGL_MAX_GS_TEXTURE_UNITS;        ///<Geometry Shader中的最大纹理单元
        extern int HGL_MAX_TCS_TEXTURE_UNITS;       ///<Tess Control Shader中的最大纹理单元
        extern int HGL_MAX_TES_TEXTURE_UNITS;       ///<Tess Evaluation Shader中的最大纹理单元
        extern int HGL_MAX_CS_TEXTURE_UNITS;        ///<Compute Shader中的最大纹理单元

        extern int HGL_MAX_TEXTURE_SIZE;            ///<最大纹理尺寸
        extern int HGL_MAX_CUBE_MAP_SIZE;           ///<最大CubeMap纹理尺寸

		/**
		* 贴图类
		*/
		class Texture
		{
		public:

			/**
			* 贴图加载预处理
			*/
			enum LoadTextureProcess:uint
			{
				ltGenMipmaps=1<<0,		//生成mipmaps
			};//enum LoadTextureProcess

		protected:

			uint texture_id;																		///<纹理ID

			uint type;																				///<贴图类型

			uint color_format;																		///<色彩格式(如RED,RG,RGB,RGBA,SRGB,SRGBA之类)
			uint video_format;																		///<显存格式

			uint min_filter,mag_filter;																///<放大和缩小过滤方式

			void *bitmap_data;																		///<原始点阵数据(不一定存在)

		public:

							Texture();																//请使用CreateTexture()来创建贴图,函数在Render.h中定义
			virtual			~Texture(){if(bitmap_data)hgl_free(bitmap_data);}

		public:

					int		GetID		    ()const{return texture_id;}								///<取得纹理ID
					uint	GetType			()const{return type;}									///<取得类型
					uint	GetColorFormat	()const{return color_format;}							///<取得颜色格式
					uint	GetVideoFormat	()const{return video_format;}							///<取得显存中的数据格式

		public:

			virtual void	SetMinFilter	(uint);													///<设置缩小过滤
			virtual void	SetMagFilter	(uint);													///<设置放大过滤

			virtual void	GetMipmapLevel	(int &,int &);											///<取得贴图mipmaps级别
		};//class Texture
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_INCLUDE
