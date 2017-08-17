#include<hgl/graph/GL/glew.h>
namespace hgl
{
    namespace graph
    {
        namespace
        {
            static GLfloat ClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
            static GLfloat ClearDepth = 1.0f;
            static GLint ClearStencil = 0;
            static GLfloat ClearAccum = 0.0f;

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
        * @param red    三原色红色量
        * @param green    三原色绿色量
        * @param blue    三原色蓝色量
        */
        void SetClearColor(float red,float green,float blue)
        {
            ClearColor[0] = red;
            ClearColor[1] = green;
            ClearColor[2] = blue;

            //glClearColor(red,green,blue,1.0f);
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

        //glClearBufferfv系列函数最低需求OpenGL 3.0
        void ClearColorBuffer()        {glClearBufferfv(GL_COLOR,0,ClearColor);}
        void ClearDepthBuffer()        {SetDepthMask(true);glClearBufferfv(GL_DEPTH,0,&ClearDepth);}
        void ClearColorDepthBuffer(){ClearColorBuffer();ClearDepthBuffer();}
//        void ClearColorDepthBuffer(){SetDepthMask(true);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);}
        void ClearStencilBuffer()    {glClearBufferiv(GL_STENCIL,0,&ClearStencil);}
        void ClearAccumBuffer()        {glClearBufferfv(GL_ACCUM,0,&ClearAccum);}
    }//namespace graph
}//namespace hgl
