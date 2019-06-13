#ifndef HGL_GUI_OBJECT_INCLUDE
#define HGL_GUI_OBJECT_INCLUDE

#include<hgl/object/VisualObject.h>
#include<hgl/type/BaseString.h>
#include<hgl/type/RectScope.h>
#include<hgl/type/Vertex2.h>
namespace hgl
{
    namespace graph
    {
        class ObjectControl;
        class TileFont;

        namespace gui
        {
            /**
            * GUI控件对齐方式
            */
            enum GuiAlign:uint
            {
                alNone,                     ///<起始定义，无意义

                alTop,                      ///<对齐最上端
                alBottom,                   ///<对齐底端
                alLeft,                     ///<对齐左边
                alRight,                    ///<对齐右边

                alClient,                   ///<使用剩下的所有空间

                alLeftTop,                  ///<左上角
                alRightTop,                 ///<右上角
                alLeftBottom,               ///<左下角
                alRightBottom,              ///<右下角

                alTopCenter,                ///<上边中间
                alLeftCenter,               ///<左边中间
                alRightCenter,              ///<右边中间
                alBottomCenter,             ///<下边中间

                alCenter,                   ///<正中间

                alEnd                       ///<结束定义，无意义
            };

            using MouseCoord=Vertex2i;

            /**
            * GUI对象是所有GUI控件的基类，它主要是使用了相对坐标设计，并且支持对齐属性，以及一些通用自动事件
            */
            class GUIObject:public VisualObject                                                              ///GUI对象
            {
                #include<hgl/gui/GuiObject.Attrib.h>

            public: //属性，供用户设置

                TileFont *          DrawFont;                                                               ///<绘制用字体

                bool                Cutout;                                                                 ///<是否做范围裁剪

                Property<GuiAlign>  Align;                                                                  ///<控件对齐方式

                Property<int>       Left;                                                                   ///<控件左边坐标
                Property<int>       Top;                                                                    ///<控件上边坐标
                Property<int>       Width;                                                                  ///<控件宽度
                Property<int>       Height;                                                                 ///<控件高度

                Property<bool>      Move;                                                                   ///<可以被移动

//              PropertyString      Hint;                                                                   ///<提示信息

                bool                AutoActive;                                                             ///<自动切换到活动状态

                void *              CustomizeData;                                                          ///<自定义数据

            public: //只读属性，供用户读取

                Property<int>       Right;                                                                  ///<控件右边界
                Property<int>       Bottom;                                                                 ///<控件底边界

                Property<int>       CenterX;                                                                ///<控件中心X
                Property<int>       CenterY;                                                                ///<控件中心Y

                Property<Vertex2i>  MenuCoord;                                                              ///<画面绝对坐标

                Property<bool>      MouseFocus;                                                             ///<鼠标是否在上面

            public: //事件

                DefEvent(void,OnEnter,(GUIObject *));                                                       ///<鼠标进入事件
                DefEvent(void,OnLeave,(GUIObject *));                                                       ///<鼠标离开事件
//              DefEvent(bool,OnHint,(GUIObject *));                                                        ///<产生提示信息

                DefEvent(bool,OnClick,(GUIObject *,int));                                                   ///<鼠标点击事件(在区域内按下并抬起)
                DefEvent(bool,OnMove,(GUIObject *));                                                        ///<控件移动事件

            public:

                GUIObject();                                                                                ///<本类构造函数
                GUIObject(const RectScope2i &);                                                             ///<本类构造函数
                GUIObject(int,int,int,int);                                                                 ///<本类构造函数
                virtual ~GUIObject()=default;                                                               ///<本类析构函数

                virtual void SetMenuCoord(const Vertex2i &);                                                ///<设置对象绝对坐标

                RectScope2i &GetViewScope(){return view_scope;}                                             ///<取得显示范围
                virtual void SetViewScope(const RectScope2i &rs);                                           ///<设置显示范围

                virtual void SetPosition(int,int);                                                          ///<设置对象位置
                virtual void SetSize(int,int);                                                              ///<设置对象尺寸
                virtual void SetLeft(int);                                                                  ///<设置对象左偏移
                virtual void SetTop(int);                                                                   ///<设置对象顶偏移
                virtual void SetWidth(int);                                                                 ///<设置对象宽
                virtual void SetHeight(int);                                                                ///<设置对象高

                Matrix4f GetObjectScaleMatrix()const
                {
                    return scale(view_scope.Width,view_scope.Height,1);
                }

                virtual void DrawObject(const Matrix4f *);
                virtual void SetScissor(int *);

                const MouseCoord &GetMouseCoord()const{return mouse_coord;}
            };//class GUIObject

            #define ON_DRAW_AND_RETURN(param)       if(OnDraw){OnDraw(param);return;}
        }//namespace gui
    }//namespace graph
}//namespace hgl
#endif//HGL_GUI_OBJECT_INCLUDE
