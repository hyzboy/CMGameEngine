#include<hgl/LogInfo.h>
#include"apiDraw.h"
#include"apiRenderable.h"
#include"apiTexture.h"
#include"apiVertexBufferObject.h"
#include"apiUniformBufferObject.h"

namespace hgl
{
	namespace graph
	{
		namespace api
		{
			bool InitTextureAPI();
		}//namespace api

		bool InitOpenGLAPI()
		{
			IF_FALSE_RETURN(api::InitTextureAPI())
			//IF_FALSE_RETURN(api::InitVertexBufferObjectAPI())
			//IF_FALSE_RETURN(api::InitUniformBufferObjectAPI())
			//IF_FALSE_RETURN(api::InitRenderableAPI())
			//IF_FALSE_RETURN(api::InitDrawAPI())

			return(true);
		}
	}//namespace graph
}//namespace api