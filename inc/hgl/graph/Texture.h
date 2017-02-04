#ifndef HGL_GRAPH_TEXTURE_INCLUDE
#define HGL_GRAPH_TEXTURE_INCLUDE

#include<hgl/graph/TextureData.h>
#include<hgl/type/BaseString.h>
namespace hgl
{
	namespace graph
	{
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
		protected:

            uint type;
            uint binding_type;

			uint texture_id;																		///<纹理ID

			uint color_format;																		///<色彩格式(如RED,RG,RGB,RGBA,SRGB,SRGBA之类)
			uint video_format;																		///<显存格式

		public:

							Texture(uint t,uint bt);
			virtual			~Texture();

		public:

					int		GetID		    ()const{return texture_id;}								///<取得纹理ID
			        uint	GetType			()const{return type;}									///<取得类型
			        uint	GetBindingType	()const{return binding_type;}							///<取得绑定类型
					uint	GetColorFormat	()const{return color_format;}							///<取得颜色格式
					uint	GetVideoFormat	()const{return video_format;}							///<取得显存中的数据格式

		public:

                    void    GenMipmaps      ();                                                     ///<生成mipmaps

		//	virtual void	GetMipmapLevel	(int &,int &)=0;										///<取得贴图mipmaps级别
		};//class Texture
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_INCLUDE
