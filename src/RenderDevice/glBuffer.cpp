#include<glew/include/GL/glew.h>
namespace hgl
{
	namespace graph
	{
		namespace
		{
			static unsigned int ClearBuffer= GL_COLOR_BUFFER_BIT
											|GL_DEPTH_BUFFER_BIT;

			static struct
			{
				GLboolean mask;
				GLboolean test;
				GLint func;
			}CurDepth;
		}//namespace

		namespace OpenGLCore
		{
			void InitBuffer()
			{
				CurDepth.test=glIsEnabled(GL_DEPTH_TEST);

				if(CurDepth.test)
					glGetIntegerv(GL_DEPTH_FUNC,&CurDepth.func);

				glGetBooleanv(GL_DEPTH_WRITEMASK,&CurDepth.mask);
			}
		}//namespace OpenGLCore

		/**
		* 开启深度测试
		* @param func 深度测试方式
		*/
		void OpenDepthTest(unsigned int func)
		{
			if(CurDepth.test)
			{
				if(CurDepth.func==func)
					return;
			}
			else
			{
				CurDepth.test=true;
				glEnable(GL_DEPTH_TEST);
			}

			CurDepth.func=func;
			glDepthFunc(func);
		}

		/**
		* 关闭深度测试
		*/
		void CloseDepthTest()
		{
			if(!CurDepth.test)return;

			CurDepth.test=false;
			glDisable(GL_DEPTH_TEST);
		}

		/**
		* 设置深度遮罩
		* @param mask 是否开启
		*/
		void SetDepthMask(bool mask)
		{
			if(CurDepth.mask==mask)return;

			if(mask)
			{
				CurDepth.mask=GL_TRUE;
				glDepthMask(GL_TRUE);
			}
			else
			{
				CurDepth.mask=GL_FALSE;
				glDepthMask(GL_FALSE);
			}
		}

		/**
		* 设置清屏时的颜色
		* @param red	三原色红色量
		* @param green	三原色绿色量
		* @param blue	三原色蓝色量
		*/
		void SetClearColor(float red,float green,float blue)
		{
			glClearColor(red,green,blue,1.0f);
		}

		/**
		* 设置清屏时的深度值
		* @param clear_depth 深度值
		*/
		void SetClearDepth(float clear_depth)
		{
			glDepthMask(GL_TRUE);
			glClearDepth(clear_depth);
		}

		/**
		* 设置清屏时清除那些缓冲区
		* @param color 颜色缓冲区
		* @param depth 深度缓冲区
		* @param stencil 模板缓冲区
		* @param accum 累积缓冲区
		*/
		void SetClearBuffer(bool color,bool depth,bool stencil,bool accum)
		{
			ClearBuffer=0;

			if(color	)ClearBuffer|=GL_COLOR_BUFFER_BIT;
			if(depth	)ClearBuffer|=GL_DEPTH_BUFFER_BIT;
			if(stencil	)ClearBuffer|=GL_STENCIL_BUFFER_BIT;
			if(accum	)ClearBuffer|=GL_ACCUM_BUFFER_BIT;
		}

		void ClearScreen()
		{
			if(ClearBuffer&GL_DEPTH_BUFFER_BIT)
				SetDepthMask(true);

			glClear(ClearBuffer);
		}

		void ClearColorBuffer()		{glClear(GL_COLOR_BUFFER_BIT);}
		void ClearDepthBuffer()		{SetDepthMask(true);glClear(GL_DEPTH_BUFFER_BIT);}
		void ClearColorDepthBuffer(){SetDepthMask(true);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);}
		void ClearStencilBuffer()	{glClear(GL_STENCIL_BUFFER_BIT);}
		void ClearAccumBuffer()		{glClear(GL_ACCUM_BUFFER_BIT);}
	}//namespace graph
}//namespace hgl
