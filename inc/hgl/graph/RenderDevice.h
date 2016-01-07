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
        class RenderableData;
        class Renderable;

		/**
		 * 渲染设备基类
		 */
		class RenderDevice																			///<渲染设备基类
		{
		public:	//视口与裁剪函数

			virtual void GetViewport(RectScope2i &);												///<取得视口
			virtual void SetViewport(const RectScope2i &);											///<设置视口

			virtual void GetRenderSize(float &,float &);											///<取得渲染尺寸

			virtual void OpenScissor(const RectScope2i &);											///<开启载剪区
			virtual void CloseScissor();															///<关闭裁剪区

		public:	//缓冲区相关

			virtual void SetClearColor(float,float,float,float=0.0f);								///<设置清屏颜色
			virtual void SetClearColor(float lum){SetClearColor(lum,lum,lum,0.0f);}					///<设置清屏颜色

			virtual void SetClearColorBuffer(bool);													///<设置是否清除颜色缓冲区
			virtual void SetClearDepthBuffer(bool);													///<设置是否清除深度缓冲区

			virtual void ClearScreen();																///<清屏
			virtual void ClearColorBuffer();														///<清除颜色缓冲区
			virtual void ClearDepthBuffer();														///<清除深度缓冲区

			virtual void OpenDepthTest(uint);

        public: //纹理相关

        public: //渲染数据相关

            virtual RenderableData *CreateRenderableData()=0;                                       ///<创建一个渲染数据
            virtual Renderable *    CreateRenderable(RenderableData *)=0;                           ///<创建一个渲染对象
		};//class RenderDevice

		/**
         * 设备类型枚举
         */
        enum RenderDeivceType
        {
            rdtNone=0,

            rdtOpenGLCore3,
            rdtOpenGLCore4,

            rdtOpenGLES1,
            rdtOpenGLES2,
            rdtOpenGLES3,
            rdtOpenGLES31,

            rdtEnd
        };//enum RenderDeivceType

		/**
         * 创建一个渲染设备
         * @param type 设备类型,默认OpenGLCore 4.x
         */
		RenderDevice *CreateRenderDevice(const RenderDeivceType &type=rdtOpenGLCore4);              ///<创建一个渲染设备
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDER_DEVICE_INCLUDE
