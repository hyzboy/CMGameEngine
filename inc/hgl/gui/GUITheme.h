#ifndef HGL_GUI_THEME_INCLUDE
#define HGL_GUI_THEME_INCLUDE

#include<hgl/type/BaseString.h>
namespace hgl
{
    namespace gui
    {
        /**
        * GUI主题服务名字空间<br>
        * 这里主要集种GUI的绘制代码
        */
        namespace theme
        {
            /**
            * 底板绘制基类
            */
            class Panel
            {
            public:

                Color4f OuterColor;                                                                     ///<外框颜色
                Color4f InnerColor;                                                                     ///<内框颜色

            public:

                Panel();
                virtual ~Panel();

                void Draw(const Matrix4f *mv);                                                          ///<绘制底板，将l/t/w/h独立传入是为了减少
            };//class Panel

            /**
            * GUITheme即“图形界面主题”，为整个GUI引擎提供原素的绘制和配置服务。
            */
            class GUITheme                                                                              ///图形界面主题
            {
            protected:

                UTF16String theme_name;                                                                 ///<主题名称

            public:

                PropertyString Name;                                                                    ///<主题名称虚拟变量

            public:

                GUITheme(const UTF16String &);
                virtual ~GUITheme();
            };//class GUITheme
        }//namespace theme
    }//namespace gui
}//namespace hgl
#endif//HGL_GUI_THEME_INCLUDE
