#include<hgl/graph/GL/glew.h>
namespace hgl
{
    namespace graph
    {
        namespace
        {
            static struct
            {
                GLenum face;
                GLenum mode;
            }CurPolygonMode;

            static struct
            {
                GLboolean cull;
                GLint mode;
            }CurCullFace;

            static GLint front_face;
        }//namespace

        namespace OpenGLCore
        {
            void InitFace()
            {
                GLint pm[2];

                glGetIntegerv(GL_POLYGON_MODE,pm);

                CurPolygonMode.face=pm[0];
                CurPolygonMode.mode=pm[1];

                //------------------

                CurCullFace.cull=glIsEnabled(GL_CULL_FACE);

                if(CurCullFace.cull)
                    glGetIntegerv(GL_CULL_FACE_MODE,&CurCullFace.mode);

                //------------------

                glGetIntegerv(GL_FRONT_FACE,&front_face);
            }
        }

        /**
        * 设置多边形绘制模式
        * @param face 多边形面
        * @param mode 绘制模式
        */
        void SetPolygonMode(unsigned int face,unsigned int mode)
        {
            if(CurPolygonMode.face==face
             &&CurPolygonMode.mode==mode)
                return;

            CurPolygonMode.face=face;
            CurPolygonMode.mode=mode;

            glPolygonMode(face,mode);
        }

        /**
        * 开启面裁剪
        * @param mode 裁剪掉的面
        */
        void OpenCullFace(unsigned int mode)
        {
            if(CurCullFace.cull)
            {
                if(CurCullFace.mode==mode)
                    return;
            }
            else
            {
                CurCullFace.cull=true;
                glEnable(GL_CULL_FACE);
            }

            CurCullFace.mode=mode;
            glCullFace(mode);
        }

        /**
        * 关闭面裁剪
        */
        void CloseCullFace()
        {
            if(!CurCullFace.cull)return;

            CurCullFace.cull=false;
            glDisable(GL_CULL_FACE);
        }

        /**
        * 设置三角面正面绘制方向
        */
        void SetFrontFace(unsigned int ff)
        {
            if(front_face==ff)return;

            front_face=ff;
            glFrontFace(ff);
        }
    }//namespace graph
}//namespace hgl
