#ifndef HGL_GRAPH_COLOR_FORMAT_INCLUDE
#define HGL_GRAPH_COLOR_FORMAT_INCLUDE

#include<glew/include/GL/glew.h>
namespace hgl
{
	namespace graph
	{
		/**
		* 顶点颜色格式枚举
		*/
		enum ColorFormat
		{
			HGL_COLOR_NONE=0,

			HGL_COLOR_ALPHA,
			HGL_COLOR_LUMINANCE,
			HGL_COLOR_LUMINANCE_ALPHA,

			HGL_COLOR_RGB,
			HGL_COLOR_RGBA,

			HGL_COLOR_END
		};//enum ColorFormat

		#define HGL_R								GL_RED
		#define HGL_RG								GL_RG
		#define HGL_RGB								GL_RGB
		#define HGL_SRGB							GL_SRGB
		#define HGL_RGBA							GL_RGBA
		#define HGL_SRGBA							GL_SRGB_ALPHA
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_COLOR_FORMAT_INCLUDE
