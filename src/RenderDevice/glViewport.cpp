#include<glew/include/GL/glew.h>
namespace hgl
{
    namespace graph
    {
        namespace
        {
            static int ScreenWidth=0;
            static int ScreenHeight=0;

            static struct
            {
                GLboolean enable;

                int left,top,width,height;
            }CurScissor;
        }//namespace

        namespace OpenGLCore
        {
            void InitScissor()
            {
                CurScissor.enable=glIsEnabled(GL_SCISSOR_TEST);

                if(CurScissor.enable)
                {
                    int v[4];
                    int s[4];

                    glGetIntegerv(GL_VIEWPORT,v);
                    glGetIntegerv(GL_SCISSOR_BOX,s);

                    CurScissor.left        =s[0];
                    CurScissor.top        =v[3]-s[1]-s[3];
                    CurScissor.width    =s[2];
                    CurScissor.height    =s[3];
                }
            }
        }//namespace OpenGLCore

        /**
        * 取得当前视口
        * @param left 左边界
        * @param top 上边界
        * @param width 宽
        * @param height 高
        */
        void GetViewport(int &left,int &top,int &width,int &height)
        {
            int v[4];

            glGetIntegerv(GL_VIEWPORT,v);

            left=v[0];
            top=v[1];
            width=v[2];
            height=v[3];
        }

        /**
        * 设置当前视口
        * @param left 左边界
        * @param top 上边界
        * @param width 宽
        * @param height 高
        */
        void SetViewport(int left,int top,int width,int height)
        {
            glViewport(left,top,width,height);

            ScreenWidth=width;
            ScreenHeight=height;

            CurScissor.enable=false;
            glDisable(GL_SCISSOR_TEST);
        }

        /**
        * 取得屏幕宽高
        */
        void GetScreenSize(int &width,int &height)
        {
            width=ScreenWidth;
            height=ScreenHeight;
        }

        /**
        * 取得屏幕宽度
        */
        int GetScreenWidth()
        {
            return ScreenWidth;
        }

        /**
        * 取得屏幕高度
        */
        int GetScreenHeight()
        {
            return ScreenHeight;
        }

        /**
        * 设置裁剪区域
        * @param left 左边界
        * @param top 顶边界
        * @param width 宽
        * @param height 高
        */
        void OpenScissor(int left,int top,int width,int height)
        {
            if(CurScissor.enable)
            {
                if(CurScissor.left  ==left
                 &&CurScissor.top   ==top
                 &&CurScissor.width ==width
                 &&CurScissor.height==height)return;        //和原来的裁剪状态一样就不设置了
            }
            else
            {
                if(left==0
                 &&top==0
                 &&width==ScreenWidth
                 &&height==ScreenHeight)
                {
                    glDisable(GL_SCISSOR_TEST);                //如果裁剪区等于全屏，则等同关闭裁剪

                    CurScissor.enable=GL_FALSE;

                    return;
                }

                glEnable(GL_SCISSOR_TEST);

                CurScissor.enable=GL_TRUE;
            }

            CurScissor.left     =left;
            CurScissor.top      =top;
            CurScissor.width    =width;
            CurScissor.height   =height;

            glScissor(left,ScreenHeight-height-top,width,height);
        }

        void CloseScissor()
        {
            if(!CurScissor.enable)return;

            CurScissor.enable=GL_FALSE;

            glDisable(GL_SCISSOR_TEST);
        }
    }//namespace graph
}//namespace hgl
