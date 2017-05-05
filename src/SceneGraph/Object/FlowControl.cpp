#include<hgl/object/FlowControl.h>

namespace hgl
{
    void FlowControl::ChangeActiveObject(FlowObject *obj)
    {
        if(active_obj)
            active_obj->UnjoinControl(this);
            
        _FlowControl<FlowObject>::ChangeActiveObject(obj);

        if(active_obj)
            active_obj->JoinControl(this);
    }

    /**
    * 调用当前控制器中的对象绘制画面
    */
    void FlowControl::Draw(const Matrix4f *mv)
    {
        if(!this->active_obj)return;

        if(!this->active_obj->CanDraw())return;

        {
            int n,count=this->objstack.GetCount();

            for(n=0;n<count;n++)
            {
                FlowObject *obj=this->objstack[n];

                if(obj->CanDraw())
                    obj->Draw(mv);
            }
        }

        this->active_obj->Draw(mv);
    }
}//namespace hgl
