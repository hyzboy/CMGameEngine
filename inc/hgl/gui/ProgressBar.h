#ifndef HGL_GUI_PROGRESS_BAR_INCLUDE
#define HGL_GUI_PROGRESS_BAR_INCLUDE

#include<hgl/type/Color4f.h>
#include<hgl/gui/GuiObject.h>
#include<hgl/VectorMath.h>
namespace hgl
{
    namespace graph
    {
        class Renderable;
        class Material;

        namespace gui
        {
            /**
            * 进度条控件
            */
            class ProgressBar:public GUIObject
            {
            protected:

                Matrix4f edge_mat;
                Matrix4f bar_mat;

            protected:

                int min_value,max_value;
                int progress;

                bool refresh_matrix;

                void InitPrivate();

                void SetViewScope(const RectScope2i &rs){GUIObject::SetViewScope(rs);refresh_matrix=true;}

            protected:

                virtual bool Proc_MouseButton(int,bool);

            public: //属性

                Color4f BackColor;                                                                      ///<底色
                Color4f ProgressBarColor;                                                               ///<进度条色

            public: //事件

                DefEvent(void,OnProgressClick,(ProgressBar *,int));                                     ///<点击事件

            public:

                ProgressBar();
                ProgressBar(int,int,int,int,int=0,int=99,int=0);
                virtual ~ProgressBar();

                void Draw(const Matrix4f *);

                int GetMinValue()const{return min_value;}
                int GetMaxValue()const{return max_value;}
                int GetProgress()const{return progress;}

                void SetMinValue(int);
                void SetMaxValue(int);
                void SetProgress(int);
            };//class ProgressBar
        }//namespace gui
    }//namespace graph
}//namespace hgl
#endif//HGL_GUI_PROGRESS_BAR_INCLUDE
