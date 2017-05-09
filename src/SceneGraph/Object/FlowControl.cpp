#include<hgl/object/FlowControl.h>

namespace hgl
{
    void FlowControl::OnClose()
    {
        if(active_obj)
            active_obj->OnClose();

        for(int i=0;i<objstack.GetCount();i++)
            objstack[i]->OnClose();
    }

    void FlowControl::OnResize      (int w,int h)
    {
        if(active_obj)
            active_obj->OnResize(w,h);
        
        for(int i=0;i<objstack.GetCount();i++)
            objstack[i]->OnResize(w,h);
    }

    void FlowControl::OnRotate      (int ang)
    {
        if(active_obj)
            active_obj->OnRotate(ang);

        for(int i=0;i<objstack.GetCount();i++)
            objstack[i]->OnRotate(ang);
    }

    bool FlowControl::OnMouseMove   (int x,int y)
    {
        if(active_obj)
            active_obj->OnMouseMove(x,y);

        for(int i=0;i<objstack.GetCount();i++)
            objstack[i]->OnMouseMove(x,y);

        return(false);
    }

    bool FlowControl::OnMouseWheel  (int x,int y)
    {
        if(active_obj)
            active_obj->OnMouseWheel(x,y);

        for(int i=0;i<objstack.GetCount();i++)
            objstack[i]->OnMouseWheel(x,y);

        return(false);
    }                                       

    bool FlowControl::OnMouseDown   (MouseButton mb)
    {
        if(active_obj)
            active_obj->OnMouseDown(mb);

        for(int i=0;i<objstack.GetCount();i++)
            objstack[i]->OnMouseDown(mb);

        return(false);
    }

    bool FlowControl::OnMouseUp     (MouseButton mb)
    {
        if(active_obj)
            active_obj->OnMouseUp(mb);

        for(int i=0;i<objstack.GetCount();i++)
            objstack[i]->OnMouseUp(mb);

        return(false);
    }

    bool FlowControl::OnMouseRepeat (MouseButton mb)
    {
        if(active_obj)
            active_obj->OnMouseRepeat(mb);

        for(int i=0;i<objstack.GetCount();i++)
            objstack[i]->OnMouseRepeat(mb);

        return(false);
    }

    bool FlowControl::OnKeyDown     (KeyboardButton kb)
    {
        if(active_obj)
            active_obj->OnKeyDown(kb);

        for(int i=0;i<objstack.GetCount();i++)
            objstack[i]->OnKeyDown(kb);

        return(false);
    }

    bool FlowControl::OnKeyUp       (KeyboardButton kb)
    {
        if(active_obj)
            active_obj->OnKeyUp(kb);

        for(int i=0;i<objstack.GetCount();i++)
            objstack[i]->OnKeyUp(kb);

        return(false);
    }

    bool FlowControl::OnKeyRepeat   (KeyboardButton kb)
    {
        if(active_obj)
            active_obj->OnKeyRepeat(kb);

        for(int i=0;i<objstack.GetCount();i++)
            objstack[i]->OnKeyRepeat(kb);

        return(false);
    }

    bool FlowControl::OnChar        (os_char ch)
    {
        if(active_obj)
            active_obj->OnChar(ch);

        for(int i=0;i<objstack.GetCount();i++)
            objstack[i]->OnChar(ch);

        return(false);
    }

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
