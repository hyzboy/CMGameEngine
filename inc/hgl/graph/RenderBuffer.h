#ifndef HGL_GRAPH_RENDER_BUFFER_INCLUDE
#define HGL_GRAPH_RENDER_BUFFER_INCLUDE

#include<hgl/type/Color3f.h>
#include<glew/include/GL/glew.h>
namespace hgl
{
	namespace graph
	{
		/**
		 * 渲染缓冲区
		 */
		class RenderBuffer
		{
		protected:

			GLuint buffer_id;

		public:

			RenderBuffer();
			RenderBuffer(int);
			virtual ~RenderBuffer();

			bool Create();																			///<创建一个渲染缓冲区
			bool Close();																			///<关闭当前渲染缓冲区
		};//class RenderBuffer

		extern RenderBuffer *PrimaryRenderBuffer;													///<缺省主渲染缓冲区
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDER_BUFFER_INCLUDE
