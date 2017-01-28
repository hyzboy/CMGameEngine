#define HGL_API_TEXTURE_INCLUDE
#define HGL_API_TEXTURE_INCLUDE

#include<hgl/graph/Texture.h>
namespace hgl
{
	namespace graph
	{
		namespace api
		{
			bool InitTextureAPI();

			Texture *CreateTexture(TextureType);

		}//namespace api
	}//namespace graph
}//namespace hgl
#endif//HGL_API_TEXTURE_INCLUDE
