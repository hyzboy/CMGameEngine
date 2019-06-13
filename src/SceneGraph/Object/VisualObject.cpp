#include<hgl/object/VisualObject.h>
// #include<hgl/object/ObjectControl.h>

namespace hgl
{
    namespace graph
    {
        VisualObject::VisualObject()
        {
            object_attrib|=oaVisual;

            visible=true;
            topmost=false;
            bottommost=false;

            Alpha=1;

            hglSetProperty(Visible,     this,VisualObject::GetVisible,      VisualObject::SetVisible    );
            hglSetProperty(TopMost,     this,VisualObject::GetTopMost,      VisualObject::SetTopMost    );
            hglSetProperty(BottomMost,  this,VisualObject::GetBottomMost,   VisualObject::SetBottomMost );
        }

        void VisualObject::SetVisible(bool v)
        {
            if(visible==v)return;

            visible=v;

//          if(super_object)
//              super_object->SetVisibleAttrib(this,visible);
        }

        void VisualObject::SetTopMost(bool tm)
        {
            if(topmost==tm)return;

            topmost=tm;

            if(topmost)
                BottomMost=false;

//          if(super_object)
//              super_object->MoveToTopLevel(this);
        }

        void VisualObject::SetBottomMost(bool bm)
        {
            if(bottommost==bm)return;

            bottommost=bm;

            if(bottommost)
                TopMost=false;

//          if(super_object)
//              super_object->MoveToBottomLevel(this);
        }

        /**
        * 对象绘制函数。如果对象没有重载这个函数，即被视为放弃绘制属性，将不会存在于对象控制器的绘制列表里
        */
        void VisualObject::Draw(const Matrix4f *)
        {
            Visible=false;
        }
    }//namespace graph
}//namespace hgl
