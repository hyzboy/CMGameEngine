#ifndef HGL_VISUALOBJECT_INCLUDE
#define HGL_VISUALOBJECT_INCLUDE

#include<hgl/object/Object.h>
namespace hgl
{
    namespace graph
    {
        /**
        * 可视对象基类，这个类定义了可视对象的基本操作方法
        */
        class VisualObject:public Object                                                            ///可视对象基类
        {
            friend class ObjectControl;

        protected:

            bool visible;
            bool topmost;
            bool bottommost;

            virtual const bool is_visual    ()const{return visible;}
            virtual const bool is_proc_event()const{return(visible&&enabled);}

        protected:

            bool GetVisible(){return visible;}
            bool GetTopMost(){return topmost;}
            bool GetBottomMost(){return bottommost;}

            virtual void SetVisible(bool);
            virtual void SetTopMost(bool);
            virtual void SetBottomMost(bool);

        public: //属性

            Property<bool>  Visible;                                                                    ///<对象是否可见
            Property<bool>  TopMost;                                                                    ///<对象是否永远在最顶层显示
            Property<bool>  BottomMost;                                                                 ///<对象是否永远在最底层显示

            float Alpha;                                                                                ///<整体Alpha透明度

        public: //方法

            VisualObject();
            virtual ~VisualObject()=default;

            virtual void Draw(const Matrix4f *);                                                        ///<对象绘制函数
            virtual void DrawObject(const Matrix4f *mv){Draw(mv);}                                      ///<对象绘制
        };//class VisualObject
    }//namespace graph
}//namespace hgl
#endif//HGL_VISUALOBJECT_INCLUDE
