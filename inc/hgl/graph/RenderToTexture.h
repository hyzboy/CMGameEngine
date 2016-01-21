#ifndef HGL_GRAPH_RENDER_TO_TEXTURE_INCLUDE
#define HGL_GRAPH_RENDER_TO_TEXTURE_INCLUDE

#include<hgl/graph/TextureFormat.h>
#include<hgl/type/Color3f.h>
namespace hgl
{
	namespace graph
	{
		class Texture2D;
		class TextureCubeMap;

		class RenderToTexture
		{
		protected:

			int viewport[4];

			bool CheckFrameBufferStatus(uint);

		public:

			RenderToTexture();
			virtual ~RenderToTexture();

			virtual bool Begin()=0;												///<开始使用
			virtual void End()=0;												///<结束使用
		};//class RenderToTexture

		class RenderToTextureColor:public RenderToTexture
		{
		protected:

			uint fbo;
			uint rb_depth;
			Texture2D *tex_color;

			GLenum draw_buffers;

			Color3f back_color;
			float init_depth;

		public:

			RenderToTextureColor(uint width,uint height,const TextureSourceFormat &,const Color3f &bc=Color3f(0,0,0),const float id=1.0f);
			virtual ~RenderToTextureColor();

			Texture2D *GetTexture(){return tex_color;}

			bool Begin();
			void End();
		};//class RenderToTextureColor

		class RenderToTextureDepth:public RenderToTexture
		{
		protected:

			uint fbo_depth;
			uint rb_depth;
			Texture2D *tex_depth;

		public:

			RenderToTextureDepth(uint width,uint height,uint depth=32);
			virtual ~RenderToTextureDepth();
		};//class RenderToTextureDepth

		class RenderToTextureColorDepth:public RenderToTexture
		{

		public:

			RenderToTextureColorDepth(uint width, uint height);
		};//class RenderToTextureColorDepth

		class RenderToTextureCubeMap:public RenderToTexture
		{
		};//class RenderToTextureCubeMap

	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDER_TO_TEXTURE_INCLUDE
