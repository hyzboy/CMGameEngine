#ifndef HGL_GRAPH_OPENGL_CORE_TEXTURE_INCLUDE
#define HGL_GRAPH_OPENGL_CORE_TEXTURE_INCLUDE

#include<hgl/graph/Texture.h>
namespace hgl
{
	namespace graph
	{
		class glTexture2D:public Texture2D
		{
		public:

			glTexture2D();
			~glTexture2D();
			
			bool	SetImage		(uint,uint,void *,uint,TSF,uint,uint);
			void	GetMipmapLevel	(int &,int &);
			int		GetImage		(void *,TSF,int);
			bool	ChangeImage		(uint,uint,uint,uint,void *,uint,TSF);

			void	SetMinFilter	(uint);
			void	SetMagFilter	(uint);
			void	SetWrapS		(uint);
			void	SetWrapT		(uint);
		};//class glTexture2D
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_OPENGL_CORE_TEXTURE_INCLUDE