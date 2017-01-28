#include<hgl/LogInfo.h>
#include"apiDraw.h"
#include"apiRenderable.h"
#include"apiTexture.h"
#include"apiVertexBufferObject.h"

namespace hgl
{
	namespace graph
	{
		bool InitOpenGLAPI()
		{
			IF_FALSE_RETURN(api::InitTextureAPI())
			IF_FALSE_RETURN(api::InitVertexBufferObjectAPI())
			IF_FALSE_RETURN(api::InitRenderableAPI())
			IF_FALSE_RETURN(api::InitDrawAPI())

			return(true);
		}
	}//namespace graph
}//namespace api