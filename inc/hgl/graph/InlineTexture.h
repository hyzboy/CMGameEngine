#ifndef HGL_GRAPH_INLINE_TEXTURE_INCLUDE
#define HGL_GRAPH_INLINE_TEXTURE_INCLUDE

namespace hgl
{
	namespace graph
	{
        class Texture1D;
        class Texture2D;

		//位于InlineRenderable.CPP中

		extern Texture1D *WhiteLine;																///<白线背景
		extern Texture1D *BlackWhiteLine;															///<黑白线

		extern Texture2D *BlackWhiteGrid;															///<黑白格贴图
		extern Texture2D *GrayWhiteGrid;                                                            ///<灰白格贴图
		extern Texture2D *BlueWhiteGrid;                                                            ///<蓝白格贴图
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_INLINE_TEXTURE_INCLUDE
