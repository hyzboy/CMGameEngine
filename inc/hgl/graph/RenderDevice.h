#ifndef HGL_GRAPH_RENDER_DEVICE_INCLUDE
#define HGL_GRAPH_RENDER_DEVICE_INCLUDE

#include<hgl/type/RectScope.h>
namespace hgl
{
	/**
	 * 图形模块名字空间
	 */
	namespace graph																					///图形模块名字空间
	{
		/**
		 * 渲染设备基类
		 */
		class RenderDevice																			///<渲染设备基类
		{
		public:	//视口与裁剪函数

			void GetViewport(RectScope2i &);														///<取得视口
			void SetViewport(const RectScope2i &);													///<设置视口

			void GetRenderSize(float &,float &);													///<取得渲染尺寸

			void OpenScissor(const RectScope2i &);													///<开启载剪区
			void CloseScissor();																	///<关闭裁剪区

		public:	//缓冲区相关

			void SetClearColor(float,float,float,float=0.0f);										///<设置清屏颜色
			void SetClearColor(float lum){SetClearColor(lum,lum,lum,0.0f);}							///<设置清屏颜色

			void SetClearColorBuffer(bool);															///<设置是否清除颜色缓冲区
			void SetClearDepthBuffer(bool);															///<设置是否清除深度缓冲区

			void ClearScreen();																		///<清屏
			void ClearColorBuffer();																///<清除颜色缓冲区
			void ClearDepthBuffer();																///<清除深度缓冲区

			void OpenDepthTest(uint);
		};//class RenderDevice
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDER_DEVICE_INCLUDE
