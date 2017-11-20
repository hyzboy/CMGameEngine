#ifndef HGL_GRAPH_RENDER_TO_TEXTURE_INCLUDE
#define HGL_GRAPH_RENDER_TO_TEXTURE_INCLUDE

#include<hgl/graph/TextureFormat.h>
#include<hgl/type/Color4f.h>
#include<hgl/type/List.h>
namespace hgl
{
	namespace graph
	{
		class Texture2D;
		class Texture2DArray;
		class TextureCubeMap;

		/**
		 * 渲染到纹理基类
		 */
		class RenderToTexture
		{
		protected:

			uint fbo;
			int viewport[4];

			bool CheckFrameBufferStatus(uint,const char *,const int);

		protected:

			virtual bool Use()=0;										///<使用当前FBO

		public:

			RenderToTexture();
			virtual ~RenderToTexture();

			bool Begin();												///<开始使用
			void End();													///<结束使用
		};//class RenderToTexture

		/**
		 * 渲染到纹理，仅颜色
		 */
		class RenderToTextureColor:public RenderToTexture
		{
		protected:

			uint rb_depth;
			Texture2D *tex_color;

			GLenum draw_buffers;

			Color4f back_color;
			float init_depth;

		public:

			RenderToTextureColor(uint width,uint height,const TextureSourceFormat &,const Color4f &bc=Color4f(0,0,0,0),const float id=1.0f);
			virtual ~RenderToTextureColor();

			Texture2D *GetTexture(){return tex_color;}

			bool Use()override;
		};//class RenderToTextureColor

		/**
		 * 渲染到纹理，仅深度
		 */
		class RenderToTextureDepth:public RenderToTexture
		{
		protected:

			Texture2D *tex_depth;

			float init_depth;

		public:

			RenderToTextureDepth(uint width,uint height,const TextureSourceFormat &,const float id=1.0f);
			virtual ~RenderToTextureDepth();

			Texture2D *GetTexture(){return tex_depth;}

			bool Use()override;
		};//class RenderToTextureDepth

		/**
		 * 渲染到纹理，颜色与深度
		 */
		class RenderToTextureColorDepth:public RenderToTexture
		{
		protected:

			Texture2D *tex_color;
			Texture2D *tex_depth;

			GLenum draw_buffers;

			Color4f back_color;
			float init_depth;

		public:

			RenderToTextureColorDepth(uint width,uint height,const TextureSourceFormat &color_tsf,const TextureSourceFormat &depth_tsf,const Color4f &bc=Color4f(0,0,0,0),const float id=1.0f);
			virtual ~RenderToTextureColorDepth();

			Texture2D *GetColorTexture(){return tex_color;}
			Texture2D *GetDepthTexture(){return tex_depth;}

			bool Use()override;
		};//class RenderToTextureColorDepth

        /**
		 * 渲染到纹理，多通道
		 */
		class RenderToTextureMultiChannel:public RenderToTexture
		{
		protected:

            uint width,height;

			Texture2D *tex_depth;
			float init_depth;
            uint rb_depth;

            Color4f back_color;

            ObjectList<Texture2D> tex_list;

            uint *attachments;

		public:

			RenderToTextureMultiChannel(uint w,uint h,const Color4f &bc=Color4f(0,0,0,0),const float id=1.0f);
			virtual ~RenderToTextureMultiChannel();

            Texture2D *AddDepth(const TextureSourceFormat &depth_tsf);
            Texture2D *AddColor(const TextureSourceFormat &color_tsf);

            bool BindComplete();            ///<绑定完成

			bool Use()override;
		};//class RenderToTextureMultiChannel

		///**
		// * 渲染到2D阵列纹理
		// */
		//class RenderToTextureLayer:public RenderToTexture
		//{
		//protected:

		//	Texture2DArray *tex_color;
		//	Texture2D *tex_depth;

		//	GLenum *draw_buffers;

		//	Color3f back_color;
		//	float init_depth;

		//public:

		//	RenderToTextureLayer(uint width,uint height,uint depth,const TextureSourceFormat &color_tsf,const TextureSourceFormat &depth_tsf,const Color3f &bc=Color3f(0,0,0),const float id=1.0f);
		//	virtual ~RenderToTextureLayer();

		//	Texture2DArray *GetColorTexture(){return tex_color;}
		//	Texture2D *		GetDepthTexture(){return tex_depth;}

		//	bool Use()override;
		//};//class RenderToTextureLayer

		//class RenderToTextureCubeMap:public RenderToTexture
		//{
		//protected:

		//	TextureCubeMap *tex;

		//	GLenum *draw_buffers;
		//};//class RenderToTextureCubeMap
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDER_TO_TEXTURE_INCLUDE
