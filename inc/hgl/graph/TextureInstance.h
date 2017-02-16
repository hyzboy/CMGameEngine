#ifndef HGL_GRAPH_TEXTURE_INSTANCE_INCLUDE
#define HGL_GRAPH_TEXTURE_INSTANCE_INCLUDE

#include<hgl/platform/Platform.h>
namespace hgl
{
    namespace graph
    {
        /**
        * 纹理实例，用于真实实现纹理的功能
        */
        class TextureInstace
        {
        protected:

            uint type;
            uint binding_type;

			uint texture_id;																		///<纹理ID

			uint color_format;																		///<色彩格式(如RED,RG,RGB,RGBA,SRGB,SRGBA之类)
			uint video_format;																		///<显存格式

        public:

            TextureInstance(uint id,uint t,uint bt);
            virtual ~TextureInstance();

		public:

					int		GetID		    ()const{return texture_id;}								///<取得纹理ID
			        uint	GetType			()const{return type;}									///<取得类型
			        uint	GetBindingType	()const{return binding_type;}							///<取得绑定类型
					uint	GetColorFormat	()const{return color_format;}							///<取得颜色格式
					uint	GetVideoFormat	()const{return video_format;}							///<取得显存中的数据格式

		public:     //mipmaps

            virtual void    GenMipmaps      ()=0;                                                   ///<生成mipmaps
			virtual void	GetMipmapLevel	(int &,int &)=0;										///<取得贴图mipmaps级别
        };//class TextureInstace
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_INSTANCE_INCLUDE
